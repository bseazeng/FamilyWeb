#ifndef _LIST__
#define _LIST__
#include <stdio.h>
#include <string.h>

struct list_head
{
	struct list_head *next;
	struct list_head *prev;
};

#define LIST_HEAD_INIT(name) {&(name),&(name)}

#define offsetof(TYPE,MEMBER) ((size_t)&((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

static inline int list_empty(struct list_head *head)
{
    return head->next == head;
}

static inline void __list_add(struct list_head *new, struct list_head *prev,struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}


static inline void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}


static inline void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

#define LIST_POISON1 ((void *) 0x00100100)
#define LIST_POISON2 ((void *) 0x00200200)

static inline void list_del(struct list_head *node)
{
    __list_del(node->prev, node->next);
    node->next = LIST_POISON1;
    node->prev = LIST_POISON2;
}



#define prefetch(x) __builtin_prefetch(x)

#define list_for_each(pos, head) \
         for (pos = (head)->next; prefetch(pos->next), pos != (head); \
                 pos = pos->next)
#define list_for_each_prev(pos,head) for(pos = (head)->prev; prefetch(pos->prev),pos != (head); pos = pos->prev)

/**  
 * list_for_each_safe   -   iterate over a list safe against removal of list entry  
 * @pos:    the &struct list_head to use as a loop counter.  
 * @n:      another &struct list_head to use as temporary storage  
 * @head:   the head for your list.  
 */  
#define list_for_each_safe(pos, n, head)\
	for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)  

#define list_entry(ptr, type, member)\
         container_of(ptr, type, member)

#endif
