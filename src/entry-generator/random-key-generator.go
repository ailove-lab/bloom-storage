package main

import (
	"flag"
	"fmt"
	"math/rand"
	// "runtime"
	"strconv"
	"time"
)

var rnd = rand.New(rand.NewSource(time.Now().UnixNano()))


func main() {

	// runtime.GOMAXPROCS(runtime.NumCPU())

	flag.Parse()
	args := flag.Args()

	cnt, err := strconv.Atoi(args[0])
	if err != nil || cnt <= 0 {
		fmt.Println("usage: generator count", err)
		return
	}

	for i := 0; i < cnt; i++ {
		fmt.Printf("%08X%08X%08X%08X\n",rnd.Uint32(),rnd.Uint32(),rnd.Uint32(),rnd.Uint32())
	}

}
