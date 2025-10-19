package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"time"

	pb "github.com/Theossr/Helikos_test/api/proto"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
)

type SimRequest struct {
	TLE struct {
		Title string `json:"title"`
		Line1 string `json:"line1"`
		Line2 string `json:"line2"`
	} `json:"tle"`
	Start_epoch_ms uint64  `json:"start_epoch_ms"`
	Duration_s     float64 `json:"duration_s"`
	Step_s         float64 `json:"step_s"`
	H_fail_km      float64 `json:"h_fail_km"`
}

func HealthHandler(w http.ResponseWriter, r *http.Request) {

	if r.Method != http.MethodGet {
		http.Error(w, "405/Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	w.WriteHeader(http.StatusOK)
	fmt.Fprintln(w, "200/OK")
}

func SimHandler(w http.ResponseWriter, r *http.Request) {

	if r.Method != http.MethodPost {
		http.Error(w, "405/Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	var sat SimRequest
	err := json.NewDecoder(r.Body).Decode(&sat)
	if err != nil {
		http.Error(w, "400/Bad request Invalid JSON", http.StatusBadRequest)
		return
	}

	conn, err := grpc.NewClient("0.0.0.0:50051", grpc.WithTransportCredentials(insecure.NewCredentials()))
	if err != nil {
		http.Error(w, "500/Internal server error", http.StatusInternalServerError)
		log.Printf("Connection error: %v", err)
		return
	}

	defer conn.Close()

	client := pb.NewSatelliteEarthSimulationServiceClient(conn)

	protoSimReq := &pb.SimRequest{

		Tle: &pb.SimRequest_TLE{

			Title: sat.TLE.Title,
			Line1: sat.TLE.Line1,
			Line2: sat.TLE.Line2,
		},

		StartEpochMs: sat.Start_epoch_ms,
		DurationS:    sat.Duration_s,
		StepS:        sat.Step_s,
		HFailKm:      sat.H_fail_km,
	}

	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()

	resp, err := client.RunSimulation(ctx, protoSimReq)
	if err != nil {
		http.Error(w, "500/Internal server error", http.StatusInternalServerError)
		log.Printf("Connection error: %v", err)
		return
	}

	json.NewEncoder(w).Encode(resp)
}

func main() {

	http.HandleFunc("/health", HealthHandler)
	http.HandleFunc("/simulate", SimHandler)

	http.ListenAndServe("localhost:8080", nil)
}
