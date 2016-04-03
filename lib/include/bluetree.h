/* 
 * Copyright (C) 2016 Floris Creyf
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef BLUETREE_H
#define BLUETREE_H

typedef struct bt_tree_tag bt_tree;

bt_tree *bt_new_tree();
void bt_delete_tree(bt_tree *tree);
void bt_generate_tree(bt_tree *tree, float *vertex_buffer,
		unsigned short *element_buffer);

#endif /* BLUETREE_H  */

