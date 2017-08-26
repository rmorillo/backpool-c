void POOL_TYPE_PREFIX(subscribe)(Pool* pool, void (*poolChange)(POOL_ITEM_TYPE* poolItem))
{
    Pool_subscribe(pool, poolChange);
}

POOL_ITEM_TYPE* POOL_TYPE_PREFIX(current)(Pool* pool)
{
    return Pool_current(pool);
}

POOL_ITEM_TYPE*  POOL_TYPE_PREFIX(previous)(Pool* pool)
{
    return Pool_previous(pool);
}
