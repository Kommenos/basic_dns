## How It Works

```
Input URL → Parse Hostname → Build DNS Message → Send via UDP → Parse Response → Print IP
```

1. **Hostname Parsing** — strips scheme and path from a URL to extract the bare hostname
2. **DNS Encoding** — encodes the hostname into DNS wire format (length-prefixed labels)
3. **Message Construction** — builds a valid DNS query with a 12-byte header, question section (`QTYPE=A`, `QCLASS=IN`)
4. **Network Transport** — opens a UDP socket and sends the query to Cloudflare's DNS at `1.1.1.1:53`
5. **Response Parsing** — navigates the answer section and prints the resolved IPv4 address

---

## Project Structure

```
.
├── dns_resolver.cpp   # Entry point
├── parser.hpp
├── parser.cpp         # URL parsing, DNS encoding, response parsing
├── network.hpp
└── network.cpp        # UDP socket, send/receive
```

---

## Build & Run

### Requirements
- C++17 or later
- Linux / macOS (uses POSIX sockets)

### Compile

```bash
g++ -std=c++17 dns_resolver.cpp parser.cpp network.cpp -o dns_resolver
```

### Usage

```bash
./dns_resolver <url>
```

### Example

```bash
./dns_resolver https://example.com/

# Output:
# Host: example.com
# IP: 93.184.216.34
```

---

## Implementation Details

| Component | Detail |
|-----------|--------|
| Transport | UDP over port 53 |
| DNS Server | Cloudflare `1.1.1.1` |
| Query Type | `A` record (IPv4) |
| Query Class | `IN` (Internet) |
| Max Response | 512 bytes (standard DNS UDP limit) |
| Transaction ID | `0xAABB` |

---

## Limitations

- Only resolves **IPv4 (A records)** — no AAAA/IPv6 support
- Returns the **first answer** only — multiple records (e.g. CDN IPs) are not iterated
- No **DNS pointer compression** handling in answer names
- No **response validation** — transaction ID and RCODE are not verified
- DNS server (`1.1.1.1`) is **hardcoded** — not configurable at runtime

---
