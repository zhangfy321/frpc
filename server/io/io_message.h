typedef struct iomessage
{
    uint64_t seq_id;

    void* buffer;

    void* extra;
}