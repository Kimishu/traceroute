package main

import (
	"fmt"
	"os"
	//third-party
	flag "github.com/spf13/pflag"
	//internal
	"go-trace/traceroute"
)

var helpCmd *bool

func Help() {
	fmt.Fprintf(os.Stdout, "Usage:\n")
	fmt.Fprintf(os.Stdout, "Commands:\n")
	fmt.Fprintf(os.Stdout, "\t--help, -h\tShows info about go-trace\n")
	fmt.Fprintf(os.Stdout, "Example:\n")
	fmt.Fprintf(os.Stdout, "\tgo-trace <link>\tShows trace to destination\n")
}

func init() {
	helpCmd = flag.BoolP("help", "h", false, "")
	flag.Usage = Help
}

func main() {
	flag.Parse()
	if *helpCmd {
		Help()
		os.Exit(1)
	} else if len(flag.Args()) != 1 {
		fmt.Println("Use -h/--help for more info!")
		os.Exit(1)
	}

	t := &traceroute.Tracer{}
	if err := t.Trace(flag.Args()[0]); err != nil {
		fmt.Println(err)
	}
}
