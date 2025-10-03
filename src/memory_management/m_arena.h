#ifndef M_ARENA
#define M_ARENA

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#define MIN_ARENA_SIZE 1024 * 1024 * 64

// BASE is the beginning of memory arena
// OFFSET is the first position that can be written to 
// (counting from base i.e. first position is base + offset)
// SIZE is the whole size of memory arena (int bytes)
typedef struct {
    void* base; 
    int64_t offset;
    int64_t size;
} M_Arena;

/// @brief initializes the memory arena to be the size of MIN_ARENA_SIZE and filled with zeros
/// @param arena pointer to the memory arena to be initialized
void m_arena_init(M_Arena* arena){
    arena->base = calloc(MIN_ARENA_SIZE, sizeof(void*));
    arena->offset = 0;
    arena->size = MIN_ARENA_SIZE;
}

/// @brief initializes the arena with given size
/// @param arena pointer to the memory arena to be initialized
/// @param size the size that the memory arena will have
void m_arena_init_size(M_Arena* arena, int64_t size){
    arena->base = calloc(size, sizeof(void*));
    arena->offset = 0;
    arena->size = size;
}

/// @brief copys data from one memory arena to another
/// @param from arena from which to copy
/// @param to arena to which to copy
void m_arena_copy_data(M_Arena* from, M_Arena* to){
    memcpy(to->base, from->base, from->size);
}

/// @brief allocates the memory in the arena
/// @param arena arena to allocate to
/// @param size the ammount of data to allocate
void* m_arena_alloc(M_Arena* arena, int64_t size){
    int64_t new_offset = arena->offset + size;
    if(new_offset > arena->size)
        m_arena_increase_size(arena, new_offset);
    
    void* data_pointer = arena->base + arena->offset;
    arena->offset = new_offset;
    return data_pointer;
}

/// @brief allocates data in the arena if there is enough space to fit
/// @param arena arena to allocate to
/// @param size the ammount of bytes to allocate
/// @return 0 if not allocated, pointer to allocation otherwise
void* m_arena_alloc_if_fits(M_Arena* arena, int64_t size){
    int64_t memory_used = arena->offset + arena->size;
    if(memory_used > arena->size) return NULL;
    return m_arena_alloc(arena, size);
}

/// @brief frees teh whole arena
/// @param arena arena to free
void m_arena_free(M_Arena* arena){
    free(arena->base);
    arena->base = 0;
    arena->size = 0;
    arena->offset = 0;
}

#endif