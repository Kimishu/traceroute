package traceroute

import (
	"fmt"
	"log"
	"net"
	"os"
	"time"
	//
	"golang.org/x/net/icmp"
	"golang.org/x/net/ipv4"
)

const maxTtl = 30

type Tracer struct {
	destinationAddr string
	destinationIPs  []string
}

func (t *Tracer) Trace(addr string) error {
	conn, err := icmp.ListenPacket("ip4:icmp", "0.0.0.0")
	if err != nil {
		return err
	}
	defer conn.Close()

	if err := t.getIP(addr); err != nil {
		return err
	}

	t.printIP()

	wm := icmp.Message{
		Type: ipv4.ICMPTypeEcho, Code: 0,
		Body: &icmp.Echo{
			ID:   os.Getpid() & 0xffff,
			Seq:  1,
			Data: nil,
		},
	}
	wb, err := wm.Marshal(nil)

	fmt.Println("\tTTL\tTime\tAddress")
	startTime := time.Now()

	for ttl := 1; ttl <= maxTtl; ttl++ {
		time.Sleep(time.Second)
		if err := conn.IPv4PacketConn().SetTTL(ttl); err != nil {
			log.Println(err)
			return err
		}

		start := time.Now()

		//IP cannot be nil, I prevent it before
		if _, err := conn.WriteTo(wb, &net.IPAddr{IP: net.ParseIP(t.destinationIPs[0])}); err != nil {
			log.Fatal(err)
		}

		rb := make([]byte, 1024)
		if err := conn.SetReadDeadline(start.Add(time.Second * 3)); err != nil {
			log.Println(err)
		}

		n, peer, err := conn.ReadFrom(rb)
		if err != nil {
			log.Println(err)
		}

		rm, err := icmp.ParseMessage(1, rb[:n])
		if rm == nil {
			fmt.Printf("Unknown route\n")
			continue
		}

		//Prefer `if` to `switch` cause of `break` operator
		if rm.Type == ipv4.ICMPTypeEchoReply {
			fmt.Printf("->\t[%d]\t%dms\t%v\n", ttl, int(time.Since(start).Milliseconds()), peer)
			break
		} else if rm.Type == ipv4.ICMPTypeTimeExceeded {
			fmt.Printf("\t[%d]\t%dms\t%v\n", ttl, int(time.Since(start).Milliseconds()), peer)
		}
	}

	fmt.Printf("Done in %d seconds\n", int(time.Since(startTime).Seconds()))

	return nil
}

func (t *Tracer) getIP(addr string) error {
	ip, err := net.LookupHost(addr)
	if err != nil {
		return fmt.Errorf("Destination doesnt exist!")
	}
	t.destinationAddr = addr
	t.destinationIPs = ip
	return nil
}

func (t *Tracer) printIP() {
	fmt.Printf("\nDestination: %s\n", t.destinationAddr)
	fmt.Printf("\t[%d] %s*\n", 1, t.destinationIPs[0])
	for i, v := range t.destinationIPs[1:] {
		fmt.Printf("\t[%d] %s\n", i+1, v)
	}
	fmt.Println()
}
