/* SilcTree tests */

#include "silcruntime.h"

SilcTree tree;

typedef struct {
  int id;
  int od;
  char *foo;
  SilcTreeHeader header;
} Foo;

#define NUM 199
Foo foo[NUM], foo2[NUM], tmp, *entry;

static int compare(void *e1, void *e2, void *context)
{
  Foo *f1 = e1, *f2 = e2;
  SILC_LOG_DEBUG(("%p %d > %p %d", f1, f1->id, f2, f2->id));
  if (f1->id > f2->id)
    return SILC_COMPARE_GREATER_THAN;
  if (f1->id < f2->id)
    return SILC_COMPARE_LESS_THAN;
  return SILC_COMPARE_EQUAL_TO;
}

int main(int argc, char **argv)
{
  SilcBool success = FALSE;
  int i;

  silc_runtime_init();

  if (argc > 1 && !strcmp(argv[1], "-d")) {
    silc_log_debug(TRUE);
    silc_log_quick(TRUE);
    silc_log_debug_hexdump(TRUE);
    silc_log_set_debug_string("*tree*");
  }

  for (i = 0; i < NUM; i++) {
    foo[i].id = i + 1;
    foo[i].od = i + 10;
  }

  for (i = 0; i < NUM; i++) {
    foo2[i].id = (i + 1) % (NUM / 4);
    foo2[i].od = (i + 10) % (NUM / 4);
  }

  /* AVL */
  SILC_LOG_DEBUG(("Create AVL tree"));
  if (!silc_tree_init(tree, SILC_TREE_AVL, compare, NULL,
		      silc_offsetof(Foo, header), TRUE))
    goto err;

  /* Populate tree */
  SILC_LOG_DEBUG(("Populate tree, %d entries", NUM));
  for (i = 0; i < NUM; i++)
    if (!silc_tree_add(tree, &foo[i]))
      goto err;

  /* Add duplicates */
  SILC_LOG_DEBUG(("Add duplicates"));
  for (i = 0; i < NUM; i++)
    if (!silc_tree_add(tree, &foo2[i]))
      goto err;

  SILC_LOG_DEBUG(("Tree has %d entries", silc_tree_count(tree)));
  if (silc_tree_count(tree) != NUM + NUM)
    goto err;

  /* Find random */
  for (i = 0; i < NUM; i++) {
    tmp.id = (silc_rand() % NUM) + 1;
    SILC_LOG_DEBUG(("Find entry %d", tmp.id));
    if ((entry = silc_tree_find(tree, &tmp)) == NULL)
      goto err;
    SILC_LOG_DEBUG(("Found entry %p %d", entry, entry->id));
  }

  /* Find non-existing */
  for (i = 0; i < 5; i++) {
    tmp.id = (silc_rand() % NUM) + (i % 2 ? -NUM - 1 : NUM + 1);
    SILC_LOG_DEBUG(("Find entry %d", tmp.id));
    if (silc_tree_find(tree, &tmp))
      goto err;
  }

  /* Enumerate in order */
  for (entry = silc_tree_enumerate(tree, NULL);
       entry;
       entry = silc_tree_enumerate(tree, entry)) {
    SILC_LOG_DEBUG(("Enum entry %d, %p", entry->id, entry));
  }

  /* Delete all */
  for (i = 0; i < NUM; i++) {
    memset(&tmp, 0, sizeof(tmp));
    tmp.id = i + 1;
    SILC_LOG_DEBUG(("Delete entry %d", tmp.id));
    if (!silc_tree_del(tree, &tmp))
      goto err;
  }

  /* Delete remaining duplicates in loop */
  while ((entry = silc_tree_enumerate(tree, NULL))) {
    if (!silc_tree_del(tree, entry))
      goto err;
  }

  SILC_LOG_DEBUG(("Tree has %d entries", silc_tree_count(tree)));
  if (silc_tree_count(tree) != 0)
    goto err;

  success = TRUE;

 err:
  SILC_LOG_DEBUG(("Testing was %s", success ? "SUCCESS" : "FAILURE"));
  fprintf(stderr, "Testing was %s\n", success ? "SUCCESS" : "FAILURE");

  silc_runtime_uninit();

  return !success;
}
