#!/usr/bin/env python3
"""Checks the steering documents: links, section references, decision IDs, language.

Run from anywhere: python3 scripts/check_docs.py
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DECISIONS = "docs/decisions.md"
# The manual is Swedish, and the glossary keeps the Swedish term column that the
# manual is written from. Every other document is English.
SWEDISH_OK = {"docs/manual.md", "docs/glossary.md"}
SWEDISH = re.compile(r"[åäöÅÄÖ]")

# "Core §9.2" and the like point into the Matter and Espressif documents.
EXTERNAL_HEADING = re.compile(
    r"(Core|Matter|ESP-IDF|ESP-Matter|Device Library|Application Cluster"
    r"|Standard Namespaces)\s*$"
)

MD_FILES = sorted(p for p in ROOT.rglob("*.md") if ".codegraph" not in p.parts)
ID = re.compile(r"\b(K[1-9]|GAP-\d{2}|TBD-(?:C)?\d{1,2})\b")
LINK = re.compile(r"\]\(([^)\s]+)\)")
CODE_PATH = re.compile(r"`([\w./-]+\.md)`")
NUM = re.compile(r"§\s*(\d+(?:\.\d+)*)(?:\s*[–—-]\s*(\d+(?:\.\d+)*))?")

errors = []


def rel(path):
    return path.relative_to(ROOT).as_posix()


def lines(path):
    return path.read_text(encoding="utf-8").splitlines()


def headings(path):
    """Numbered and named headings in a Markdown file."""
    numbers, names = set(), set()
    for line in lines(path):
        match = re.match(r"#{2,4}\s+(.+)$", line)
        if not match:
            continue
        title = re.sub(r"[`*]", "", match.group(1)).strip()
        number = re.match(r"(\d+(?:\.\d+)*)\.?\s", title + " ")
        if number:
            numbers.add(number.group(1))
        names.add(title.lower())
    return numbers, names


def resolve(source, target):
    """A Markdown path, tried relative to the repo root and to the file itself."""
    for candidate in (ROOT / target, source.parent / target):
        if candidate.exists():
            return candidate
    by_name = [p for p in ROOT.rglob(Path(target).name) if ".codegraph" not in p.parts]
    return by_name[0] if len(by_name) == 1 else None


def check_links(path):
    for number, line in enumerate(lines(path), 1):
        for target in LINK.findall(line):
            if target.startswith(("http://", "https://", "mailto:", "#")):
                continue
            bare = target.split("#")[0]
            if bare and not (path.parent / bare).exists():
                errors.append(f"{rel(path)}:{number}: broken link {target}")
        for target in CODE_PATH.findall(line):
            if not resolve(path, target):
                errors.append(f"{rel(path)}:{number}: does not exist: {target}")


def check_sections(path):
    """Every "§N" is checked against the named document, or against this one."""
    text = path.read_text(encoding="utf-8")
    own_numbers, _ = headings(path)
    for match in re.finditer(r"(?<![\w.])§\s*([^§\n]{1,60})", text):
        # The document a section reference belongs to is the one named last
        # before it, e.g. "`specification.md` – behavior (§3–9)".
        before = " ".join(text[: match.start()].splitlines()[-2:])
        if EXTERNAL_HEADING.search(before):
            continue
        named = re.findall(r"[\w./-]+\.md", before)
        if named:
            target = resolve(path, named[-1])
            if target is None:
                continue  # an unresolvable path is reported by the link check
        elif own_numbers:
            target = path
        else:
            line = text.count("\n", 0, match.start()) + 1
            errors.append(
                f"{rel(path)}:{line}: §{match.group(1).strip()} names no document"
            )
            continue
        verify_sections(path, target, "§" + match.group(1), match.start(), text)


def verify_sections(source, target, fragment, offset, text):
    line = text.count("\n", 0, offset) + 1
    numbers, names = headings(target)
    for number, last in NUM.findall(fragment):
        for value in {number, last} - {""}:
            if value not in numbers:
                errors.append(
                    f"{rel(source)}:{line}: §{value} is not a heading"
                    f" in {rel(target)}"
                )
    for candidate in re.findall(r"§\s*([^\W\d_][\w&/ -]*)", NUM.sub("", fragment)):
        words = candidate.strip(" .,;:").split()
        # "§Topic map, then ..." – try the longest heading-like prefix.
        matched = False
        for length in range(len(words), 0, -1):
            phrase = " ".join(words[:length]).lower()
            if any(phrase in heading for heading in names):
                matched = True
                break
        if not matched:
            errors.append(
                f"{rel(source)}:{line}: §{candidate.strip()} is not a heading"
                f" in {rel(target)}"
            )


def check_ids(path):
    if rel(path) == DECISIONS:
        return
    defined = set(ID.findall((ROOT / DECISIONS).read_text(encoding="utf-8")))
    for number, line in enumerate(lines(path), 1):
        for found in ID.findall(line):
            if found not in defined:
                errors.append(
                    f"{rel(path)}:{number}: {found} is not defined in {DECISIONS}"
                )


def check_language(path):
    if rel(path) in SWEDISH_OK:
        return
    # shortcut: only Swedish letters are detected, not Swedish words spelled
    # without them; upgrade to a word list if that starts to matter.
    for number, line in enumerate(lines(path), 1):
        if SWEDISH.search(line):
            errors.append(f"{rel(path)}:{number}: Swedish text outside manual.md")


def main():
    for document in MD_FILES:
        check_links(document)
        check_sections(document)
        check_ids(document)
        check_language(document)

    if errors:
        print("\n".join(errors))
        print(f"\n{len(errors)} problem(s)")
        return 1

    print(f"{len(MD_FILES)} documents checked")
    return 0


if __name__ == "__main__":
    sys.exit(main())
