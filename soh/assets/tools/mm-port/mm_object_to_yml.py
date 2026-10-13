#!/usr/bin/env python3
"""
MM port: turn Majora's Mask object asset lists into Ship of Harkinian extraction configs.

For each object it reads the Majora decomp's asset XML (from a 2 Ship 2 Harkinian checkout) and
the player's own Majora's Mask (USA) 1.0 ROM, and writes:

  soh/assets/yml/mm_ntsc_u_1-0/objects/<object>.yml   Torch config, every path under mm/
  soh/assets/mm/objects/<object>/<object>.h           C header, asset paths rewritten to mm/

The ROM is only read to find the object's offset and to scan its display lists for vertex blocks;
nothing from it is written out. Asset kinds the Ocarina extractor can't build yet (texture
animations, keyframe animations, ...) are skipped and reported, so actors needing them can be
held back.

Usage:
  mm_object_to_yml.py --mm <2ship2harkinian checkout> --rom <MM US 1.0 .z64> --soh <Shipwright checkout> OBJECT [...]
"""

import argparse
import os
import re
import struct
import sys
import xml.etree.ElementTree as ET

DMADATA = 0x1A500  # Majora's Mask (USA) 1.0 file table

# Majora XML element -> how to write it. Anything not listed is skipped and reported.
SUPPORTED = {"DList", "Texture", "Limb", "Skeleton", "Animation", "Collision", "Array", "Blob", "Mtx"}


def yaz0(src):
    size = struct.unpack(">I", src[4:8])[0]
    out = bytearray()
    i = 16
    while len(out) < size:
        code = src[i]
        i += 1
        for b in range(8):
            if len(out) >= size:
                break
            if code & (0x80 >> b):
                out.append(src[i])
                i += 1
            else:
                b1, b2 = src[i], src[i + 1]
                i += 2
                dist = ((b1 & 0xF) << 8 | b2) + 1
                n = b1 >> 4
                if n == 0:
                    n = src[i] + 0x12
                    i += 1
                else:
                    n += 2
                for _ in range(n):
                    out.append(out[-dist])
    return bytes(out)


def file_table(rom, names):
    table = {}
    for i, name in enumerate(names):
        vs, ve, ps, pe = struct.unpack(">4I", rom[DMADATA + 16 * i : DMADATA + 16 * i + 16])
        table[name] = (vs, ve, ps, pe)
    return table


def load_file(rom, entry):
    vs, ve, ps, pe = entry
    if pe == 0:  # stored uncompressed
        return rom[ps : ps + (ve - vs)]
    return yaz0(rom[ps:])


def scan_dlists(data, seg, starts):
    """Follow display lists inside this file; return vertex loads and nested display lists."""
    vtx, nested, seen = [], set(), set()
    todo = list(starts)
    while todo:
        off = todo.pop()
        if off in seen or off + 8 > len(data):
            continue
        seen.add(off)
        while off + 8 <= len(data):
            w0, w1 = struct.unpack(">II", data[off : off + 8])
            op = w0 >> 24
            off += 8
            if op == 0x01:  # G_VTX (F3DEX2)
                if (w1 >> 24) == seg:
                    vtx.append((w1 & 0xFFFFFF, (w0 >> 12) & 0xFF))
            elif op == 0xDE:  # G_DL
                if (w1 >> 24) == seg:
                    nested.add(w1 & 0xFFFFFF)
                    todo.append(w1 & 0xFFFFFF)
                if (w0 >> 16) & 0xFF == 1:  # branch, no return
                    break
            elif op == 0xDF:  # G_ENDDL
                break
    return vtx, nested


def group_vertices(loads, declared_starts, file_size):
    """Merge vertex loads into contiguous blocks that don't run into other declared assets."""
    spans = sorted((a, a + 16 * n) for a, n in loads if n > 0)
    blocks = []
    for start, end in spans:
        if blocks and start <= blocks[-1][1] and not any(blocks[-1][0] < d < end and d >= blocks[-1][1] for d in declared_starts):
            blocks[-1][1] = max(blocks[-1][1], end)
        else:
            blocks.append([start, end])
    return [(s, min(e, file_size)) for s, e in blocks]


def convert(obj, args, rom, table):
    xml_path = os.path.join(args.mm, "mm/assets/xml/N64_US/objects", obj + ".xml")
    root = ET.parse(xml_path).getroot()
    files = root.findall("File")
    if len(files) != 1:
        return None, ["%s: %d <File> entries, only single-file objects are supported" % (obj, len(files))]
    f = files[0]
    seg = int(f.get("Segment", "6"))
    entry = table[obj]
    data = load_file(rom, entry)

    out = []
    skipped = []
    declared = {}  # offset -> name
    dlists = []
    vtx_arrays = []
    for el in f:
        tag = el.tag
        name = el.get("Name")
        off = int(el.get("Offset", "0"), 16)
        if tag not in SUPPORTED:
            skipped.append("%s (%s)" % (name, tag))
            continue
        declared[off] = name
        lines = ["%s:" % name]
        if tag == "DList":
            lines += ["  type: GFX", "  offset: 0x%X" % off, "  symbol: %s" % name]
            dlists.append(off)
        elif tag == "Texture":
            lines += ["  type: TEXTURE", "  offset: 0x%X" % off, "  symbol: %s" % name,
                      "  format: %s" % el.get("Format").upper(),
                      "  width: %s" % el.get("Width"), "  height: %s" % el.get("Height")]
        elif tag == "Limb":
            lines += ["  type: OOT:LIMB", "  offset: 0x%X" % off, "  symbol: %s" % name,
                      "  limb_type: %s" % el.get("Type")]
        elif tag == "Skeleton":
            lines += ["  type: OOT:SKELETON", "  offset: 0x%X" % off, "  symbol: %s" % name,
                      "  limb_type: %s" % el.get("LimbType"), "  skel_type: %s" % el.get("Type")]
        elif tag == "Animation":
            lines += ["  type: OOT:ANIMATION", "  offset: 0x%X" % off, "  symbol: %s" % name]
        elif tag == "Collision":
            lines += ["  type: OOT:COLLISION", "  offset: 0x%X" % off, "  symbol: %s" % name]
        elif tag == "Mtx":
            lines += ["  type: OOT:MTX", "  offset: 0x%X" % off, "  symbol: %s" % name]
        elif tag == "Blob":
            lines += ["  type: BLOB", "  offset: 0x%X" % off, "  symbol: %s" % name,
                      "  size: %s" % el.get("Size")]
        elif tag == "Array":
            kinds = {c.tag for c in el}
            if kinds == {"Vtx"}:
                lines += ["  type: OOT:ARRAY", "  offset: 0x%X" % off, "  symbol: %s" % name,
                          "  count: %s" % el.get("Count"), "  array_type: VTX",
                          "  zero_flag: true", "  null_cross_file: true"]
                vtx_arrays.append((off, off + 16 * int(el.get("Count"))))
            elif kinds == {"Vector"} and all(c.get("Type") == "s16" and c.get("Dimensions") == "3" for c in el):
                lines += ["  type: OOT:ARRAY", "  offset: 0x%X" % off, "  symbol: %s" % name,
                          "  count: %s" % el.get("Count"), "  array_type: Vec3s"]
            else:
                skipped.append("%s (Array of %s)" % (name, ",".join(sorted(kinds))))
                del declared[off]
                continue
        out.append("\n".join(lines))

    # Vertex blocks the XML leaves implicit: everything the display lists load that isn't declared.
    loads, nested = scan_dlists(data, seg, dlists)
    extra = 0
    for n_off in sorted(nested):
        if n_off not in declared:
            name = "%s_DL_%06X" % (obj, n_off)
            declared[n_off] = name
            out.append("%s:\n  type: GFX\n  offset: 0x%X\n  symbol: %s" % (name, n_off, name))
            extra += 1
    uncovered = [(a, n) for a, n in loads if not any(s <= a < e for s, e in vtx_arrays)]
    for start, end in group_vertices(uncovered, sorted(declared), len(data)):
        if start in declared:
            continue
        name = "%sVtx_%06X" % (obj, start)
        out.append("%s:\n  type: OOT:ARRAY\n  offset: 0x%X\n  symbol: %s\n  count: %d\n  array_type: VTX\n"
                   "  zero_flag: true\n  null_cross_file: true" % (name, start, name, (end - start) // 16))
        extra += 1

    header = ["# Majora's Mask %s, generated by soh/assets/tools/mm-port/mm_object_to_yml.py." % obj,
              "# Offsets come from the MM decomp's assets/xml/N64_US/objects/%s.xml; vertex blocks" % obj,
              "# are found by scanning the display lists. Paths are under mm/ so they never collide",
              "# with Ocarina's files.",
              ":config:",
              "  directory: mm/objects/%s" % obj,
              "  segments:",
              "    - [ %d, 0x%08X ]" % (seg, entry[2])]
    if obj != "gameplay_keep" and seg != 4:
        header += ["  external_files:", "    - mm_ntsc_u_1-0/objects/gameplay_keep.yml"]
    yml = "\n".join(header) + "\n\n" + "\n\n".join(out) + "\n"
    return (yml, extra), skipped


def convert_header(obj, args):
    src = os.path.join(args.mm, "mm/assets/objects", obj, obj + ".h")
    text = open(src, encoding="utf-8").read()
    text = text.replace('"__OTR__objects/', '"__OTR__mm/objects/')
    guard = "OBJECTS_%s_H" % obj.upper()
    text = text.replace(guard, "MM_" + guard)
    note = ("// Majora's Mask %s, extracted into mm.o2r (see soh/assets/yml/mm_ntsc_u_1-0/objects/%s.yml).\n"
            "// Copied from 2 Ship 2 Harkinian's generated header with asset paths moved under mm/.\n" % (obj, obj))
    return note + text


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--mm", required=True, help="2 Ship 2 Harkinian checkout")
    ap.add_argument("--rom", required=True, help="Majora's Mask (USA) 1.0 ROM")
    ap.add_argument("--soh", required=True, help="Shipwright checkout")
    ap.add_argument("objects", nargs="+")
    args = ap.parse_args()

    rom = open(args.rom, "rb").read()
    names = open(os.path.join(args.mm, "mm/assets/extractor/filelists/mm.txt")).read().split()
    table = file_table(rom, names)

    yml_dir = os.path.join(args.soh, "soh/assets/yml/mm_ntsc_u_1-0/objects")
    hdr_root = os.path.join(args.soh, "soh/assets/mm/objects")
    status = 0
    for obj in args.objects:
        result, skipped = convert(obj, args, rom, table)
        if result is None:
            print("FAIL  %s: %s" % (obj, "; ".join(skipped)))
            status = 1
            continue
        yml, extra = result
        os.makedirs(yml_dir, exist_ok=True)
        open(os.path.join(yml_dir, obj + ".yml"), "w").write(yml)
        os.makedirs(os.path.join(hdr_root, obj), exist_ok=True)
        open(os.path.join(hdr_root, obj, obj + ".h"), "w").write(convert_header(obj, args))
        note = (" skipped: " + ", ".join(skipped)) if skipped else ""
        print("OK    %s (+%d found by scanning)%s" % (obj, extra, note))
    return status


if __name__ == "__main__":
    sys.exit(main())
