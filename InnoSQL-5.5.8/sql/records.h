#ifndef SQL_RECORDS_H
#define SQL_RECORDS_H 
/* Copyright (C) 2008 Sun/MySQL

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#ifdef USE_PRAGMA_INTERFACE
#pragma interface                      /* gcc class implementation */
#endif
#include <my_global.h>                /* for uint typedefs */

struct st_join_table;
class handler;
struct TABLE;
class THD;
class SQL_SELECT;

/**
  A context for reading through a single table using a chosen access method:
  index read, scan, etc, use of cache, etc.

  Use by:
  READ_RECORD read_record;
  init_read_record(&read_record, ...);
  while (read_record.read_record())
  {
    ...
  }
  end_read_record();
*/

struct READ_RECORD
{
  typedef int (*Read_func)(READ_RECORD*);
  typedef void (*Unlock_row_func)(st_join_table *);
  typedef int (*Setup_func)(struct st_join_table*);

  TABLE *table;                                 /* Head-form */
  handler *file;
  TABLE **forms;                                /* head and ref forms */
  Unlock_row_func unlock_row;
  Read_func read_record;
  THD *thd;
  SQL_SELECT *select;
  uint cache_records;
  uint ref_length,struct_length,reclength,rec_cache_size,error_offset;
  uint index;
  uchar *ref_pos;				/* pointer to form->refpos */
  uchar *record;
  uchar *rec_buf;                /* to read field values  after filesort */
  uchar	*cache,*cache_pos,*cache_end,*read_positions;
  struct st_io_cache *io_cache;
  bool print_error, ignore_not_found_rows;

public:
  READ_RECORD() {}
};

void init_read_record(READ_RECORD *info, THD *thd, TABLE *reg_form,
		      SQL_SELECT *select, int use_record_cache,
                      bool print_errors, bool disable_rr_cache);
void init_read_record_idx(READ_RECORD *info, THD *thd, TABLE *table,
                          bool print_error, uint idx, bool reverse);
void end_read_record(READ_RECORD *info);

void rr_unlock_row(st_join_table *tab);

#endif /* SQL_RECORDS_H */
