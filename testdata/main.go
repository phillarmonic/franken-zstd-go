package main

import (
	"log"

	"github.com/caddyserver/caddy/v2/cmd"

	// Import the zstd extension
	_ "github.com/phillarmonic/franken-zstd-go"
)

func main() {
	log.Println("Starting FrankenPHP with zstd extension...")
	cmd.Main()
}
