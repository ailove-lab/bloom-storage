package main

import (
	"bytes"
	"encoding/binary"
	"flag"
	"fmt"
	"math/rand"
	"runtime"
	"strconv"
	"time"
)

var rnd = rand.New(rand.NewSource(time.Now().UnixNano()))

func randomKey() []byte{
	buf := new(bytes.Buffer)
	for i := 0; i <= 3; i++ {
		err := binary.Write(buf, binary.LittleEndian, rnd.Uint32())
		if err != nil {
			fmt.Println("binary.Write failed:", err)
		}
	}
	return buf.Bytes()
}


func main() {

	runtime.GOMAXPROCS(runtime.NumCPU())

	flag.Parse()
	args := flag.Args()

	cnt, err := strconv.Atoi(args[0])
	if err != nil || cnt <= 0 {
		fmt.Println("usage: generator count", err)
		return
	}

	for i := 0; i < cnt; i++ {
		fmt.Println(randomKey())
	}

}
