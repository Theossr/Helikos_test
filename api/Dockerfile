FROM golang:1.24.9 AS builder

WORKDIR /app

COPY go.mod go.sum ./
RUN go mod download

COPY . .

RUN go build -o api main.go

FROM ubuntu:24.04

WORKDIR /app

RUN apt-get update && apt-get install -y ca-certificates curl && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/api .

# REST API port
EXPOSE 8080 

CMD ["./api"]