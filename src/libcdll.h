#ifndef __LIBCDLL_H__
#define __LIBCDLL_H__

#include <stddef.h>

struct cdll_node
{
        struct cdll_node *next;
        struct cdll_node *previous;

        void             *data;
};

typedef struct cdll_node *CDLL;

void   cdll_init                     (CDLL *list,
                                      void *data);

CDLL   cdll_first                    (const CDLL *list);

CDLL   cdll_last                     (const CDLL *list);

size_t cdll_length                   (const CDLL *list);

void   cdll_push                     (CDLL *list,
                                      void *data);

void   cdll_foreach                  (const CDLL *list,
                                      void      (*function) (void*, void*),
                                      void       *user_data);

void   cdll_reverse_foreach          (const CDLL *list,
                                      void      (*function) (void*, void*),
                                      void       *user_data);

void*  cdll_pop                      (CDLL *list);

void   cdll_add                      (CDLL *list,
                                      void *data);

void*  cdll_remove_first             (CDLL *list);

void   cdll_free                     (CDLL *list);

void   cdll_deep_free                (CDLL  *list,
                                      void (*function) (void*));

CDLL   cdll_find                     (const CDLL *list,
                                      const void *data);

CDLL   cdll_deep_find                (const CDLL *list,
                                      const void *data,
                                      int       (*compare)(const void*, const void*));

int    cdll_data_position            (const CDLL *list,
                                      const void *data);

int    cdll_deep_data_position       (const CDLL *list,
                                      const void *data,
                                      int       (*compare)(const void*, const void*));

void*  cdll_delete_node              (CDLL *list,
                                      CDLL  node);

int    cdll_position                 (const CDLL *list,
                                      const CDLL  node);

CDLL   cdll_copy                     (const CDLL *list);

CDLL   cdll_deep_copy                (const CDLL *list,
                                      void*     (*function)(const void*, void*),
                                      void       *user_data);

CDLL   cdll_nth                      (const CDLL *list,
                                      size_t      nth_node);

void*  cdll_nth_data                 (const CDLL *list,
                                      size_t      nth_data);

void*  cdll_remove_data              (CDLL       *list,
                                      const void *data);

void*  cdll_deep_remove_data         (CDLL       *list,
                                      const void *data,
                                      int       (*compare) (const void*, const void*));

#endif // __LIBCDLL_H__
