# Matter 1.6.1 local reference

Local copies of the normative Matter 1.6.1 documents, kept only to check
attribute IDs, types, constraints, cluster revisions and device type
requirements. They are not part of the build and are not committed.

| Document | File | Covers |
| --- | --- | --- |
| Core Specification 1.6.1, 23-27349 | `23-27349-012_Matter-1.6.1-Core-Specification.pdf` | Data model, interaction model, commissioning, security, Basic Information and OTA Requestor clusters |
| Application Cluster Specification 1.6.1, 23-27350 | `23-27350-011_Matter-1.6.1-Application-Cluster-Specification.pdf` | Cluster definitions: Identify, Flow Measurement, Valve Configuration and Control, and all application clusters |
| Device Library Specification 1.6.1, 23-27351 | `23-27351-011_Matter-1.6.1-Device-Library-Specification.pdf` | Device types, including Water Valve and OTA Requestor, and their required clusters |
| Standard Namespaces 1.6.1, 23-31936 | `23-31936-009_Matter-1.6.1-Standard-Namespaces.pdf` | Semantic tag namespaces (not used by the current features) |

## Text extracts

`text/*.txt` holds one extract per PDF. Each page starts with a marker such as

```text
===== PDF page 419 / printed page 415 =====
```

so a search hit can be turned into a PDF page immediately. Table rows are kept
on one line, which makes grep useful on attribute tables. Regenerate after
replacing a PDF:

```sh
cd docs/reference/external/matter-1.6.1 && python3 extract_text.py
```

The script needs `pymupdf` and `pymupdf4llm`. The extracts are an index only:
verify anything normative on the PDF page before relying on it or quoting it.
Tables still extract imperfectly, and a wrapped cell can spill onto the next
line.

## Where the relevant definitions are

Page numbers are PDF pages, matching the marker lines. The printed page number
is four lower in the Core, Application Cluster and Device Library documents and
six lower in the Standard Namespaces document, so use the marker when a section
reference matters.

| Topic | Document | Section | PDF page |
| --- | --- | --- | --- |
| Water Valve device type, ID `0x0042` | Device Library | 5.6 | 70 |
| Water Valve cluster requirements and firmware update recommendation | Device Library | 5.6.4, 5.6.5 | 71–72 |
| OTA Requestor device type | Device Library | 2.3 | 37 |
| Base Device Type, conformance tags | Device Library | 1.1 | 27 |
| Basic Information cluster, ID `0x0028` | Core | 11.1 | 793 |
| OTA Software Update Requestor cluster, ID `0x002A` | Core | 11.20.7 | 1031 |
| Identify cluster, ID `0x0003` | Application Cluster | 1.2 | 34 |
| Flow Measurement cluster, ID `0x0404` | Application Cluster | 2.5 | 192 |
| Valve Configuration and Control cluster, ID `0x0081` | Application Cluster | 4.6 | 419 |
| Data model, cluster/attribute/command tables | Core | 7 | 471 |
| Interaction model (read, write, invoke, subscribe) | Core | 8 | 563 |
| Endpoint composition and device type rules | Core | 9.2 | 614 |

Within a cluster section the order is stable: revision history, classification,
cluster ID, features, data types, status codes, attributes, commands, events.
So `4.6.7` is the Valve Configuration and Control attribute table and `4.6.8`
its commands.

## Searching

```sh
# an attribute or command ID inside one document
rg -n "0x0004" docs/reference/external/matter-1.6.1/text/23-27350*.txt

# a named item; table rows keep the ID and name on one line
rg -n "CurrentState" docs/reference/external/matter-1.6.1/text/23-27350*.txt

# a section heading, to jump to a cluster
rg -n "^#+ 4\.6\." docs/reference/external/matter-1.6.1/text/23-27350*.txt
```

Then open the PDF at the page from the nearest marker to confirm the entry.

## Files here

| File | Tracked | Contents |
| --- | --- | --- |
| `INDEX.md` | yes | This index |
| `extract_text.py` | yes | Regenerates the text extracts from the PDFs |
| `*.pdf` | no | Specification PDFs, not redistributable |
| `text/*.txt` | no | Generated extracts, not redistributable |

The `.gitignore` keeps the PDFs and the extracts out of the repository. Only
this index and the extraction script are committed, so a fresh clone has the
lookup map and the tool but must obtain the PDFs from the Connectivity
Standards Alliance.

## Licence

The Connectivity Standards Alliance specification documents are not freely
redistributable. Do not commit the PDFs or the generated text to a public
repository, and do not paste specification prose into issues, PRs or other
tracked documents beyond short citations with a section reference.

## Related

- [`../esp-idf/INDEX.md`](../esp-idf/INDEX.md) – the Espressif side: ESP-IDF 6.1
  and ESP-Matter documentation.
- `docs/specification/architecture.md` §5 – how the clusters are used by PSMD.
- The `esp-idf` MCP server (`.opencode/esp-idf-mcp.sh`, registered in
  `opencode.jsonc`) builds, flashes and monitors firmware; it does not answer
  API questions. Those belong here and in the ESP-IDF documentation.
