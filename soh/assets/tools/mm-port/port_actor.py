#!/usr/bin/env python3
"""
MM port: copy Majora's Mask actors into Ship of Harkinian.

For each overlay (e.g. ovl_En_Mkk) this copies the actor from a 2 Ship 2 Harkinian checkout into
soh/src/overlays/actors/mm/<overlay>/ and rewrites it for Ocarina's engine:

  - Names: EnMkk -> MmEnMkk, ENMKK_ -> MMENMKK_, overlay func_8XXXXXXX -> MmEnMkk_func_8XXXXXXX,
    header guards prefixed, so nothing clashes with Ocarina actors of the same name.
  - Profile: `ActorProfile En_Mkk_Profile` becomes `ActorInit Mm_En_Mkk_InitVars` with id 0 and
    gameplay_keep as its object (ActorDB assigns the id; assets are loaded by path).
  - Actor fields: speed -> speedXZ, playerHeightRel -> yDistToPlayer, ... (see ACTOR_FIELDS).
    Statements touching Majora-only fields (hintId, halfDaysBits, audioFlags) are dropped.
  - Colliders: ColliderElement -> ColliderInfo, elem -> info, atDmgInfo -> toucher, ...
  - Damage: damage tables and collider damage masks are remapped slot by slot to Ocarina's layout.
  - Includes: object headers point at soh/assets/mm/objects; mm_compat.h is included last.

It then lists the ported actors in soh/soh/Enhancements/mm-port/MMPortActors.inc, which
MMPort.cpp turns into ActorDB entries.

Usage:
  port_actor.py --mm <2ship2harkinian checkout> --soh <Shipwright checkout> ovl_En_X [...]
  port_actor.py --mm ... --soh ... --regen-list      (rebuild MMPortActors.inc from what's ported)
"""

import argparse
import os
import re
import sys

# Majora damage slot feeding each Ocarina slot (index = Ocarina slot / damage flag bit).
#   OoT: nut, stick, slingshot, explosive, boomerang, arrow, hammer swing, hookshot,
#        kokiri/master/giant slash, fire/ice/light arrow, 3 unused arrows, fire/ice/light magic,
#        shield, mirror ray, kokiri/giant/master spin, kokiri/giant/master jump, unk, unblockable,
#        hammer jump, unk
#   MM:  nut 0, stick 1, arrow 5, explosives 3, boomerang 4, goron punch 8, hookshot 7, sword 9,
#        fire/ice/light arrow 11-13, shield 20, light ray 21, spin 24, goron pound 10, ...
DMG_SLOT_FROM_MM = [0, 1, 5, 3, 4, 5, 8, 7, 9, 9, 9, 11, 12, 13, 5, 5, 5, 11, 12, 13,
                    20, 21, 24, 24, 24, 9, 9, 9, 28, 29, 10, 31]

# Actor struct members renamed between the games (Majora name -> Ocarina name).
ACTOR_FIELDS = {
    "objectSlot": "objBankIndex",
    "attentionRangeType": "targetMode",
    "sfxId": "sfx",
    "lockOnArrowOffset": "targetArrowOffset",
    "speed": "speedXZ",
    "terminalVelocity": "minVelocityY",
    "depthInWater": "yDistToWater",
    "playerHeightRel": "yDistToPlayer",
    "cullingVolumeDistance": "uncullZoneForward",
    "cullingVolumeScale": "uncullZoneScale",
    "cullingVolumeDownward": "uncullZoneDownward",
    "isLockedOn": "isTargeted",
}
# Actor members Majora has and Ocarina doesn't; statements using them are dropped.
MM_ONLY_ACTOR_FIELDS = ["hintId", "halfDaysBits", "audioFlags"]

# Collider names (Majora -> Ocarina).
COLLIDER_NAMES = [
    (r"\bColliderElementDamageInfoATInit\b", "ColliderTouch"),
    (r"\bColliderElementDamageInfoACInit\b", "ColliderBumpInit"),
    (r"\bColliderElementDamageInfoAT\b", "ColliderTouch"),
    (r"\bColliderElementDamageInfoAC\b", "ColliderBump"),
    (r"\bColliderElementInit\b", "ColliderInfoInit"),
    (r"\bColliderElement\b", "ColliderInfo"),
    (r"(\.|->)atDmgInfo\b", r"\1toucher"),
    (r"(\.|->)acDmgInfo\b", r"\1bumper"),
    (r"(\.|->)elemMaterial\b", r"\1elemType"),
    (r"(\.|->)atElemFlags\b", r"\1toucherFlags"),
    (r"(\.|->)acElemFlags\b", r"\1bumperFlags"),
    (r"(\.|->)atHitElem\b", r"\1atHitInfo"),
    (r"(\.|->)acHitElem\b", r"\1acHitInfo"),
    (r"(\.|->)colMaterial\b", r"\1colType"),
    (r"(\.|->)elem\b", r"\1info"),
    (r"(elements\[[^\]]*\])\.base\b", r"\1.info"),
]


def camel(ovl):
    """ovl_En_Mkk -> EnMkk"""
    return "".join(p for p in ovl[len("ovl_"):].split("_"))


def remap_mask(mm_mask):
    oot = 0
    for bit, src in enumerate(DMG_SLOT_FROM_MM):
        if mm_mask & (1 << src):
            oot |= 1 << bit
    return oot


def remap_damage_tables(text):
    def fix(m):
        entries = re.findall(r"(/\*[^*]*\*/\s*)?(DMG_ENTRY\([^)]*\))", m.group(2))
        if len(entries) != 32:
            return m.group(0)
        vals = [e[1] for e in entries]
        lines = ["    /* OoT slot %2d <- MM slot %2d */ %s," % (i, s, vals[s]) for i, s in enumerate(DMG_SLOT_FROM_MM)]
        return m.group(1) + "\n" + "\n".join(lines) + "\n};"
    return re.sub(r"(DamageTable \w+(?:\[\w*\])? = \{)(.*?)\n\};", fix, text, flags=re.S)


def remap_collider_masks(text):
    # Damage masks sit in collider element inits as `{ 0xXXXXXXXX, 0xYY, 0xZZ }`.
    def fix_block(block):
        return re.sub(r"\{ *0x([0-9A-Fa-f]{1,8}) *, *(0x[0-9A-Fa-f]+|\d+) *, *(0x[0-9A-Fa-f]+|\d+) *\}",
                      lambda m: "{ 0x%08X, %s, %s }" % (remap_mask(int(m.group(1), 16)), m.group(2), m.group(3)),
                      block)
    return re.sub(r"(static Collider\w*Init\w* \w+(?:\[\w*\])? = \{.*?\n\};)", lambda m: fix_block(m.group(1)),
                  text, flags=re.S)


def drop_mm_only_statements(text):
    out = []
    for line in text.split("\n"):
        if any(re.search(r"(\.|->)%s\b" % f, line) for f in MM_ONLY_ACTOR_FIELDS) and line.rstrip().endswith(";") \
                and not re.search(r"\b(if|while|for|return)\b", line):
            out.append(re.match(r"\s*", line).group(0) + "// MM port: dropped (Majora-only field): " + line.strip())
        else:
            out.append(line)
    return "\n".join(out)


def rewrite_profile(text, ovl, prefix):
    name = ovl[len("ovl_"):]

    def fix(m):
        body = m.group(1)
        parts = [p.strip() for p in re.sub(r"/\*\*/", "", body).split(",") if p.strip()]
        if len(parts) != 9:
            return m.group(0)
        parts[0] = "0 /* assigned by ActorDB */"
        parts[3] = "OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */"
        return "ActorInit Mm_%s_InitVars = {\n    %s,\n};" % (name, ",\n    ".join(parts))
    return re.sub(r"ActorProfile %s_Profile = \{(.*?)\};" % re.escape(name), fix, text, flags=re.S)


def port_text(text, ovl, is_c, own_funcs, own_fields=frozenset()):
    base = camel(ovl)
    up = base.upper()
    prefix = "Mm" + base

    text = re.sub(r"\b%s(?=[A-Z_]|\b)" % base, prefix, text)
    text = re.sub(r"\b%s_" % up, "MM%s_" % up, text)
    for f in sorted(own_funcs, key=len, reverse=True):
        text = re.sub(r"\b%s\b" % f, "%s_%s" % (prefix, f), text)
    text = re.sub(r"\b(Z_\w+_H)\b", r"MM_\1", text)

    # Object headers -> soh/assets/mm/objects; other actors -> their ported copies.
    text = re.sub(r'#include "(?:assets/)?objects/(\w+)/(\w+)\.h"', r'#include "mm/objects/\1/\2.h"', text)
    text = re.sub(r'#include "overlays/actors/(ovl_\w+)/', r'#include "overlays/actors/mm/\1/', text)
    # 2 Ship 2 Harkinian's own headers, decomp-only helpers, and Majora engine headers whose API
    # mm_compat.h provides.
    text = re.sub(r'^#include "(2s2h/[^"]*|GameInteractor/[^"]*|BenPort\.h|BenGui/[^"]*|Enhancements/[^"]*|'
                  r'ShipUtils\.h|prevent_bss_reordering\d*\.h|z64quake\.h|z64rumble\.h)".*$',
                  r'// MM port: dropped include \1', text, flags=re.M)

    for mm, oot in ACTOR_FIELDS.items():
        if mm in own_fields:
            # The actor's own struct has a field of this name: only rename accesses through an Actor.
            text = re.sub(r"(\bactor\.|\bactor->|\bthisx->|\.actor\.|->actor\.)%s\b" % mm, r"\1%s" % oot, text)
        else:
            text = re.sub(r"(\.|->)%s\b" % mm, r"\1%s" % oot, text)
        text = re.sub(r"(ICHAIN_\w+\()%s\b" % mm, r"\1%s" % oot, text)
    for pat, rep in COLLIDER_NAMES:
        text = re.sub(pat, rep, text)
    text = drop_mm_only_statements(text)

    if not is_c:
        # Types the header needs (ColliderSphere, TexturePtr, ...) come from the compat layer too.
        incs = list(re.finditer(r'^#include .*$', text, flags=re.M))
        if incs:
            at = incs[-1].end()
            text = text[:at] + '\n#include "mm_compat.h" // MM port' + text[at:]
    if is_c:
        text = rewrite_profile(text, ovl, prefix)
        text = remap_damage_tables(text)
        text = remap_collider_masks(text)
        # mm_compat.h goes after the last include: some Majora names shadow Ocarina ones.
        incs = list(re.finditer(r'^#include .*$', text, flags=re.M))
        if incs:
            at = incs[-1].end()
            text = text[:at] + '\n#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones' + text[at:]
    return text


def own_overlay_funcs(texts):
    """func_8XXXXXXX names defined by this overlay (VRAM >= 0x80800000), not engine functions."""
    names = set()
    for t in texts:
        names |= {m for m in re.findall(r"\b(func_80[89A-F][0-9A-F]{5})\s*\(", t)}
    return names


def description(text):
    m = re.search(r"\* Description: (.*)", text)
    return m.group(1).strip() if m else ""


def port(ovl, args):
    src = os.path.join(args.mm, "mm/src/overlays/actors", ovl)
    dst = os.path.join(args.soh, "soh/src/overlays/actors/mm", ovl)
    files = [f for f in sorted(os.listdir(src)) if f.endswith((".c", ".h"))]
    texts = {f: open(os.path.join(src, f), encoding="utf-8").read() for f in files}
    funcs = own_overlay_funcs(texts.values())
    # Fields declared in the actor's own structs (header and .c), e.g. `f32 speed;`.
    own_fields = set()
    for t in texts.values():
        own_fields |= set(re.findall(r"^\s*(?:/\*[^*]*\*/\s*)?[A-Za-z_][\w ]*[\w\*]\s+\**(\w+)(?:\[[^\]]*\])*;", t, flags=re.M))
    os.makedirs(dst, exist_ok=True)
    for f, t in texts.items():
        out = port_text(t, ovl, f.endswith(".c"), funcs, own_fields)
        banner = "// MM port: generated from the Majora's Mask decomp's %s/%s by port_actor.py.\n" % (ovl, f)
        open(os.path.join(dst, f), "w", encoding="utf-8").write(banner + out)
    return [f for f in files if f.endswith(".c")]


def regen_list(args):
    root = os.path.join(args.soh, "soh/src/overlays/actors/mm")
    lines = ["// Generated by soh/assets/tools/mm-port/port_actor.py. One line per ported Majora's Mask actor:",
             "// MM_ACTOR(InitVars symbol, ActorDB name, description)"]
    for ovl in sorted(os.listdir(root)) if os.path.isdir(root) else []:
        d = os.path.join(root, ovl)
        desc = ""
        for f in sorted(os.listdir(d)):
            if f.endswith(".c"):
                t = open(os.path.join(d, f), encoding="utf-8").read()
                desc = desc or description(t)
        name = ovl[len("ovl_"):]
        desc = desc.replace("\\", "\\\\").replace('"', '\\"')
        lines.append('MM_ACTOR(Mm_%s_InitVars, "Mm_%s", "MM: %s")' % (name, name, desc or name))
    path = os.path.join(args.soh, "soh/soh/Enhancements/mm-port/MMPortActors.inc")
    open(path, "w").write("\n".join(lines) + "\n")
    return len(lines) - 2


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--mm", required=True)
    ap.add_argument("--soh", required=True)
    ap.add_argument("--regen-list", action="store_true")
    ap.add_argument("overlays", nargs="*")
    args = ap.parse_args()
    for ovl in args.overlays:
        port(ovl, args)
        print("ported %s" % ovl)
    n = regen_list(args)
    print("MMPortActors.inc lists %d actors" % n)


if __name__ == "__main__":
    main()
