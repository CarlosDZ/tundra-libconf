#ifndef TUNDRA_CONF_H
#define TUNDRA_CONF_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TCONF_NAME_MAX 32
#define TCONF_VALUE_MAX 256
#define TCONF_ITEMS_MAX 512
#define TCONF_BLOCKS_MAX 128
#define TCONF_LINE_MAX 512

/* A "key = value" line. block is the index of its block, -1 at top level. A
 * quoted value has its escapes resolved and may hold any byte but NUL: len is
 * its length. */
struct tconf_item {
	int block;
	char key[TCONF_NAME_MAX];
	char value[TCONF_VALUE_MAX];
	size_t len;
	int quoted;
	int line;
};

struct tconf_block {
	char name[TCONF_NAME_MAX];
	int line;
};

/* A parsed file. Its size is part of the ABI. */
struct tconf {
	struct tconf_item items[TCONF_ITEMS_MAX];
	int n;
	struct tconf_block blocks[TCONF_BLOCKS_MAX];
	int nblocks;
	int err_line;
	char err[96];
};

const char *tconf_version(void);

/* 0, or -1 with err and err_line set. What each block and key mean is up to
 * the caller. */
int tconf_parse(const char *text, size_t len, struct tconf *g);

#ifdef __cplusplus
}
#endif

#endif
