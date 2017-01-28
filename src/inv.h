#ifndef INCLUDED_INV_H
#define INCLUDED_INV_H

#include "obj.h"

/* Helper Module for Managing Object Inventories (inv)
 * Used by equip, pack and quiver modules. Could also
 * be used for shop inventories.
 *
 * Object Ownership: Any objects added to the inventory
 * are copied. Clients own the original while this module
 * owns the copies. Filtered inventories must outlive
 * the parent (source) inventory.
 */

typedef int slot_t; /* Slots are 1..max ('if (slot) ...' is a valid idiom) */
                    /* Slots may be empty (unused) */
typedef void (*slot_f)(slot_t slot);

typedef struct inv_s inv_t, *inv_ptr; /* Hidden/Abstract */

/* Creation */
#define INV_STORE 0x0001
extern inv_ptr inv_alloc(int max, int flags); /* max=0 is unbounded */
extern inv_ptr inv_copy(inv_ptr src);
extern inv_ptr inv_filter(inv_ptr src, obj_p p);
extern void    inv_free(inv_ptr inv);

/* Adding, Removing and Sorting */
extern slot_t  inv_add(inv_ptr inv, obj_ptr obj); /* Copy obj to next avail slot ... no combining */
extern void    inv_add_at(inv_ptr inv, obj_ptr obj, slot_t slot); /* equip specifies the slot when wielding */
extern slot_t  inv_combine(inv_ptr inv, obj_ptr obj);
extern int     inv_combine_ex(inv_ptr inv, obj_ptr obj); /* Combine obj with inventory objects. Adds left over to new slot.
                                                            Modifies obj->number and returns number added to inv */
extern void    inv_remove(inv_ptr inv, slot_t slot);
extern void    inv_clear(inv_ptr inv);
extern bool    inv_optimize(inv_ptr inv);
extern bool    inv_sort(inv_ptr inv);
extern void    inv_swap(inv_ptr inv, slot_t left, slot_t right);

extern bool    inv_can_combine(inv_ptr inv, obj_ptr obj);
extern slot_t  inv_next_free_slot(inv_ptr inv);

/* Iterating, Searching and Accessing Objects (Predicates are always optional) */
extern obj_ptr inv_obj(inv_ptr inv, slot_t slot); /* NULL if slot is not occupied */
extern slot_t  inv_first(inv_ptr inv, obj_p p);
extern slot_t  inv_next(inv_ptr inv, obj_p p, slot_t prev_match); /* Begins new search at prev_match + 1 */
extern slot_t  inv_last(inv_ptr inv, obj_p p);
extern slot_t  inv_find_art(inv_ptr inv, int which); /* equip module wants to know if a certain artifact is being worn */
extern slot_t  inv_find_ego(inv_ptr inv, int which);
extern slot_t  inv_find_obj(inv_ptr inv, int tval, int sval);
extern void    inv_for_each(inv_ptr inv, obj_f f); /* apply f to each non-null object */
extern void    inv_for_each_that(inv_ptr inv, obj_f f, obj_p p); /* apply f to each object that p accepts */
extern void    inv_for_each_slot(inv_ptr inv, slot_f f); /* apply f to all slots, empty or not */
extern slot_t  inv_random_slot(inv_ptr inv, obj_p p); /* used for disenchantment, cursing, rusting, inventory damage, etc */

/* Properties of the Entire Inventory */
extern int     inv_weight(inv_ptr inv, obj_p p); /* Pass NULL for total weight */
extern int     inv_count(inv_ptr inv, obj_p p); /* Sum(obj->number) for all objects p accepts */
extern int     inv_count_slots(inv_ptr inv, obj_p p); /* Sum(1) for all objects p accepts */
extern int     inv_max_slots(inv_ptr inv); /* from inv_alloc(max) */

/* Savefiles */
extern void    inv_load(inv_ptr inv, savefile_ptr file);
extern void    inv_save(inv_ptr inv, savefile_ptr file);

#endif