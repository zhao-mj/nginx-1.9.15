
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;
    //创建一个ngx_list_t结构
    list = ngx_palloc(pool, sizeof(ngx_list_t));
    if (list == NULL) {
        return NULL;
    }
     //初始化链表
    if (ngx_list_init(list, pool, n, size) != NGX_OK) {
        return NULL;
    }

    return list;
}


void *
ngx_list_push(ngx_list_t *l)
{
    void             *elt;
    ngx_list_part_t  *last;

    last = l->last;
    //当前数组元素个数已达到上限
    if (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */
        //重新分配一个ngx_list_part_t
        last = ngx_palloc(l->pool, sizeof(ngx_list_part_t));
        if (last == NULL) {
            return NULL;
        }
        //为数组分配新空间
        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        if (last->elts == NULL) {
            return NULL;
        }

        last->nelts = 0;
        //last下一个数组指针
        last->next = NULL;
        //修改链表最后一个数组的next指针
        l->last->next = last;
        //重置链表last指针
        l->last = last;
    }
    //新元素添加的位置
    elt = (char *) last->elts + l->size * last->nelts;
    //last数组元素+1
    last->nelts++;

    return elt;
}
