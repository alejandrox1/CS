# include "free_space_indexer.h"

// A better implementation that provides
// the same functionalify as this one can
// use red-black trees for each of the three
// collections size, left_size, and left_void.
//
// Such an implementation makes alloc, and free
// run in O(nlogn) time, where n is
// free_store_item_count.

typedef struct node node_t ;

struct node {
  uint32_t key ;
  uint32_t satelite ;
  node_t * next ;
} ;

typedef struct {
  node_t *   free_store ;
  node_t * * free_store_item  ;
  uint32_t     free_store_index ;
  uint32_t     free_store_count ;
  node_t *   size ;
  node_t *   left_size ;
  node_t *   left_void ;
} indexer_data_t ;

static int
free_space_indexer_impl_size_search_min2
  ( free_space_indexer_t * indexer
  , uint32_t size
  , uint32_t * left
  , uint32_t * right
  , uint32_t * id
  ) {
  indexer_data_t * indexer_data = ((indexer_data_t*)indexer->address_begin);
  node_t * mo , * aq ;
  aq = 0 ; mo = indexer_data->size ;
  while ( 0 != mo ) {
    if ( mo->key >= size ) {
      if (0 == aq) {
        aq=mo;
      } else {
        if (aq->key>mo->key) {
          aq=mo;
        }
      }
    }
    mo=mo->next;
  }
  if ( 0 == aq ) {
    return - 1 ;
  }
  * left = aq->satelite;
  * right= aq->key+aq->satelite-1;
  * id =(uint32_t)aq;
  return 0;
}

static int
free_space_indexer_impl_size_add
  ( free_space_indexer_t * indexer
  , uint32_t left
  , uint32_t right
  , uint32_t * id
  ) {
  indexer_data_t * XX = ((indexer_data_t*)indexer->address_begin);
  if ( XX->free_store_index == XX->free_store_count ) {
    return -1 ;
  }
  node_t * ix = XX->free_store_item[XX->free_store_index] ;
  ++ XX->free_store_index ;
  ix->key=right-left+1;
  ix->satelite=left;
  ix->next=XX->size;
  XX->size=ix;
  * id = ((uint32_t)ix);
  return 0 ;
}

static int
free_space_indexer_impl_size_del
  ( free_space_indexer_t * indexer
  , uint32_t id
  ) {
  indexer_data_t * XX=((indexer_data_t*)indexer->address_begin);
  if ( 0 == XX->size ) {
    return -1 ;
  }
  node_t * ma = ((node_t*)id);
  if ( XX->size==ma ) {
    -- XX->free_store_index;
    XX->free_store_item[XX->free_store_index]=ma;
    XX->size=XX->size->next;
    return 0 ;
  }
  node_t * az , * ke ;
  az = XX->size ; ke = az->next ;
  while ( (ke != 0) && (ke != ma) ) {
    az=ke; ke=az->next ;
  }
  if ( 0 == ke ) {
    return -1 ;
  }
  -- XX->free_store_index;
  XX->free_store_item[XX->free_store_index]=ma;
  az->next=ke->next;
  return 0;
}

static int
free_space_indexer_impl_left_size_search
  ( free_space_indexer_t * indexer
  , uint32_t left
  , uint32_t * right
  , uint32_t * id
  ) {
  indexer_data_t * XX = ((indexer_data_t*)indexer->address_begin);
  node_t * mo = XX->left_size;
  while ( 0 != mo ) {
    if ( left == mo->key ) {
      node_t * e3 = ((node_t*)mo->satelite);
      * right=left+e3->key-1;
      * id = (uint32_t)e3;
      return 0 ;
    }
    mo=mo->next;
  }
  return - 1 ;
}

static int
free_space_indexer_impl_left_size_search_left
  ( free_space_indexer_t * indexer
  , uint32_t * left
  , uint32_t  right
  , uint32_t * id
  ) {
  indexer_data_t * XX = ((indexer_data_t*)indexer->address_begin);
  node_t * mo = XX->left_size;
  node_t * e3 ;
  uint32_t rr ;
  while ( 0 != mo ) {
    e3=((node_t*)mo->satelite);
    rr = mo->key+e3->key-1;
    if (right==rr) {
      * left = mo->key;
      * id = (uint32_t)e3;
      return 0;
    }
    mo=mo->next;
  }
  return -1 ;
}

static int
free_space_indexer_impl_left_size_add
  ( free_space_indexer_t * indexer
  , uint32_t id
  ) {
  indexer_data_t * XX = ((indexer_data_t*)indexer->address_begin);
  if ( XX->free_store_index == XX->free_store_count ) {
    return -1 ;
  }
  node_t * ix = XX->free_store_item[XX->free_store_index] ;
  ++ XX->free_store_index ;
  ix->key=((node_t*)id)->satelite;
  ix->satelite=id;
  ix->next=XX->left_size;
  XX->left_size=ix;
  return 0 ;
}

static int
free_space_indexer_impl_left_size_del
  ( free_space_indexer_t * indexer
  , uint32_t left
  ) {
  indexer_data_t * XX=((indexer_data_t*)indexer->address_begin);
  if ( 0 == XX->left_size ) {
    return -1 ;
  }
  if ( XX->left_size->key==left ) {
    -- XX->free_store_index;
    XX->free_store_item[XX->free_store_index]=XX->left_size;
    XX->left_size=XX->left_size->next;
    return 0 ;
  }
  node_t * az , * ke ;
  az = XX->left_size ; ke = az->next ;
  while ( (ke != 0) && (ke->key != left) ) {
    az=ke; ke=az->next ;
  }
  if ( 0 == ke ) {
    return -1 ;
  }
  -- XX->free_store_index;
  XX->free_store_item[XX->free_store_index]=ke;
  az->next=ke->next;
  return 0;
}

static int
free_space_indexer_impl_left_void_search
  ( free_space_indexer_t * indexer
  , uint32_t left
  , uint32_t * right
  ) {
  indexer_data_t * XX = ((indexer_data_t*)indexer->address_begin);
  node_t * mo = XX->left_void;
  while ( 0 != mo ) {
    if ( left == mo->key ) {
      * right=mo->satelite;
      return 0 ;
    }
    mo=mo->next;
  }
  return - 1 ;
}

static int
free_space_indexer_impl_left_void_add
  ( free_space_indexer_t * indexer
  , uint32_t left
  , uint32_t right
  ) {
  indexer_data_t * XX = ((indexer_data_t*)indexer->address_begin);
  if ( XX->free_store_index == XX->free_store_count ) {
    return -1 ;
  }
  node_t * ix = XX->free_store_item[XX->free_store_index] ;
  ++ XX->free_store_index ;
  ix->key=left;
  ix->satelite=right;
  ix->next=XX->left_void;
  XX->left_void=ix;
  return 0 ;
}

static int
free_space_indexer_impl_left_void_del
  ( free_space_indexer_t * indexer
  , uint32_t left
  ) {
  indexer_data_t * XX=((indexer_data_t*)indexer->address_begin);
  if ( 0 == XX->left_void ) {
    return -1 ;
  }
  if ( XX->left_void->key==left ) {
    -- XX->free_store_index;
    XX->free_store_item[XX->free_store_index]=XX->left_void;
    XX->left_void=XX->left_void->next;
    return 0 ;
  }
  node_t * az , * ke ;
  az = XX->left_void ; ke = az->next ;
  while ( (ke != 0) && (ke->key != left) ) {
    az=ke; ke=az->next ;
  }
  if ( 0 == ke ) {
    return -1 ;
  }
  -- XX->free_store_index;
  XX->free_store_item[XX->free_store_index]=ke;
  az->next=ke->next;
  return 0;
}

void
free_space_indexer_impl_initialize
  ( free_space_indexer_t * indexer ) {
  uint32_t address_begin = indexer->address_begin ;
  uint32_t address_end   = indexer->address_end ;
  indexer_data_t * indexer_data = ((indexer_data_t*)address_begin);
  indexer_data->free_store = ((node_t*)(address_begin+sizeof(indexer_data_t)));
  indexer_data->free_store_index = 0 ;
  indexer_data->free_store_count = (address_end-((uint32_t)indexer_data->free_store))/(sizeof(node_t)+sizeof(node_t*));
  indexer_data->free_store_item  = ((node_t**)(address_begin+sizeof(indexer_data_t)+indexer_data->free_store_count*sizeof(node_t)));
  indexer_data->size=0;
  indexer_data->left_size=0;
  indexer_data->left_void=0;
  // Make free store items point to free store locations.
  uint32_t fa , re ;
  re = indexer_data->free_store_count ;
  fa = 0 ;
  while ( re != fa ) {
    indexer_data->free_store_item[fa]= & indexer_data->free_store[fa];
    ++ fa ;
  }
}

void
free_space_indexer_impl_export
  ( free_space_indexer_t * indexer ) {
  indexer->initialize            = free_space_indexer_impl_initialize ;
  indexer->size_search_min2      = free_space_indexer_impl_size_search_min2 ;
  indexer->size_add              = free_space_indexer_impl_size_add ;
  indexer->size_del              = free_space_indexer_impl_size_del ;
  indexer->left_size_search      = free_space_indexer_impl_left_size_search ;
  indexer->left_size_search_left = free_space_indexer_impl_left_size_search_left ;
  indexer->left_size_add         = free_space_indexer_impl_left_size_add ;
  indexer->left_size_del         = free_space_indexer_impl_left_size_del ;
  indexer->left_void_search      = free_space_indexer_impl_left_void_search ;
  indexer->left_void_add         = free_space_indexer_impl_left_void_add ;
  indexer->left_void_del         = free_space_indexer_impl_left_void_del ;
}

