/**
 * @file semicolons.cpp
 * Removes extra semicolons
 *
 * @author  Ben Gardner
 * @license GPL v2+
 *
 * $Id: combine.cpp 804 2007-07-25 01:06:08Z bengardner $
 */
#include "uncrustify_types.h"
#include "chunk_list.h"
#include "ChunkStack.h"
#include "prototypes.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cctype>
#include <cassert>


static void remove_semicolon(chunk_t *pc)
{
   /* TODO: do we want to shift stuff back a column? */
   chunk_del(pc);
}

/**
 * Removes superfluous semicolons:
 *  - after brace close whose parent is IF, ELSE, SWITCH, WHILE, FOR
 *  - after another semicolon where parent is not FOR
 */
void remove_extra_semicolons(void)
{
   chunk_t *pc;
   chunk_t *next;
   chunk_t *prev;

   pc = chunk_get_head();
   while (pc != NULL)
   {
      next = chunk_get_next_ncnl(pc);

      if ((pc->type == CT_SEMICOLON) &&
          ((prev = chunk_get_prev_ncnl(pc)) != NULL))
      {
         if ((prev->type == CT_BRACE_CLOSE) &&
             ((prev->parent_type == CT_IF) ||
              (prev->parent_type == CT_ELSE) ||
              (prev->parent_type == CT_SWITCH) ||
              (prev->parent_type == CT_WHILE) ||
              (prev->parent_type == CT_FOR) ||
              (prev->parent_type == CT_FUNC_DEF)))
         {
            remove_semicolon(pc);
         }
         else if ((prev->type == CT_SEMICOLON) &&
                  (prev->parent_type != CT_FOR))
         {
            remove_semicolon(pc);
         }
      }

      pc = next;
   }
}