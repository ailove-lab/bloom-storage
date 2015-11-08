package main

/*
    
    На входе:
        1) Бинарный кеш записей (hash) 
           cache_ID = key_bin: value_bin
        2) Индекс сегментов (hash) 
           index_ID = segment_bin: segment_txt
        3) Статистика по частотности сегментов (zset) 
           segments_stat_ID = segment_bin: count


    Алгоритм bloom упаковки
        1) Расчитываем длину фильтрoв по segment_stat
        2) Для всех записей кеша
            а) Для каждого сегмента записи устанавливаем бит в соответсвующем фильтре

*/

func main() {

}
