#include <tundra/conf.h>

#include <string.h>

#include "test.h"

static struct tconf g;

int main(int argc, char **argv) {
	const char *text = "allow {\n port = 22\n}\nname = \"a b\"\n";

	CHECK(argc == 2 && strcmp(tconf_version(), argv[1]) == 0,
	      "the library reports the version of metapkg");
	CHECK(tconf_parse(text, strlen(text), &g) == 0 && g.nblocks == 1 &&
	          g.n == 2 && strcmp(g.blocks[0].name, "allow") == 0 &&
	          g.items[0].block == 0 && strcmp(g.items[0].value, "22") == 0 &&
	          g.items[1].block == -1 && g.items[1].quoted &&
	          strcmp(g.items[1].value, "a b") == 0,
	      "a file is parsed through the shared library");
	CHECK(tconf_parse("a {\n", 4, &g) < 0 && g.err_line == 1 && g.err[0],
	      "an error carries its line and its text");
	return test_summary();
}
