#ifndef LIB_vector_H_
#define LIB_vector_H_

typedef struct mLibVectorElement_t
{
    void *data;
    size_t size;
} mLibVectorElement_t;

typedef struct
{
    mLibVectorElement_t *items;
    size_t elementCount;
} mLibVector_t;

extern void mLibVector_Add(mLibVector_t *const vector, void const *const data, size_t const size);
extern void mLibVector_AddFront(mLibVector_t *const vector, void const *const data, size_t const size);
extern void mLibVector_AddBack(mLibVector_t *const vector, void const *const data, size_t const size);
extern void mLibVector_Pop(mLibVector_t *const vector, size_t const id_start, size_t const id_stop);
extern void mLibVector_PopOne(mLibVector_t *const vector, size_t const id);
extern void mLibVector_PopFirst(mLibVector_t *const vector);
extern void mLibVector_PopLast(mLibVector_t *const vector);
extern mLibVector_t mLibVector_GetSubList(mLibVector_t *const vector, size_t const id_start, size_t const id_stop);

#endif /* LIB_vector_H_ */
