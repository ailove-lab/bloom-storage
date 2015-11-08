# bloom-storage
Bloom-filter based storage

# redis db description

buff     - input buffer, temporary storage [list] "key\t(junk) seg1/seg2/.../segN"
cache_ID - current binary cache [hash] key_bin: value_bin
index_ID - current index for cache [hash] segment_bin: segment_txt
stat_ID  - segment statistics [zset]
