# tundra-libconf

Library with the configuration grammar of Tundra OS, works on any Linux. Every
Tundra tool with a configuration file uses this one format (ADR-033):
`tundra-networkd` (`*.tnc`, `interfaces`) and `tundra-icewall` (`*.rules`)
among them. It parses a file into blocks and `key = value` lines; what each
block and key mean is up to the program.

A pure function over a buffer of the caller: no allocation, no input or output,
no global state.

## Grammar

```
# a comment
top = 1

match {
	interface = enp8s0     # a bare value: trimmed, inner spaces kept
	ssid = "caf\xc3\xa9"   # a quoted value: any byte but NUL
}
```

| Rule | |
|------|-|
| Lines | One `key = value`, `name {` or `}` per line; blank lines and `#` comments are skipped. At most 512 bytes; LF or CRLF |
| Names | Keys and block names: `[A-Za-z0-9_.-]`, up to 31 bytes |
| Blocks | Not nested. The same name may open several blocks |
| Bare value | Up to the end of the line or a `#`, without quotes inside, not empty |
| Quoted value | Between `"`, with `\"`, `\\` and `\xHH`; `\x00` is refused. Nothing but a comment after the closing quote |
| Limits | 512 lines with a value and 128 blocks per file, 255 bytes per value |

## Usage

```c
#include <tundra/conf.h>

static struct tconf g;

if (tconf_parse(text, len, &g) < 0)
	fprintf(stderr, "line %d: %s\n", g.err_line, g.err);
for (int i = 0; i < g.n; i++)
	use(g.items[i].block < 0 ? "" : g.blocks[g.items[i].block].name,
	    g.items[i].key, g.items[i].value);
```

Link with `-ltundra-conf`. `struct tconf` is large: keep it static.

| Function | What it does |
|----------|--------------|
| `tconf_parse` | Parses a whole file; `0`, or `-1` with `err` and `err_line` |
| `tconf_version` | Version of the library |

## Build and install

```
make
make check
make install
```

Installs `libtundra-conf.so.0`, the `libtundra-conf.so` link and
`<tundra/conf.h>`. `make check` runs the unit tests with ASan and UBSan, a
program linked to the shared library and the list of exported symbols
(`tests/abi/symbols`).

The soname changes when a released function or struct does.

## License

GPL-3.0-or-later.
