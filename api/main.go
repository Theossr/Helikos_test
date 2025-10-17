package main

import (
	"encoding/json",
	"net/http",
	"fmt"
)

type SimRequest struct {

	tle struct {
		title string `json:"title"`
		line1 string `json:"line1"`
		line2 string `json:"line2"`
	} `json:"tle"`
	start_epoch_ms uint32 `json:"start_epoch_ms"`
	duration_s float32 `json:"duration_s"`
	step_s float32 `json:"step_s"`
	h_fail_km float32 `json:"h_fail_km"`
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

	var sat SatSim
	err := json.NewDecoder(r.Body).Decode(&sat)
	if err != nil {
		http.Error(w, "400/Bad request Invalid JSON", http.StatusBadRequest)
		return
	}
}

func main() {

	http.HandleFunc("/health", HealthHandler)
	http.HandleFunc("/simulate", SimRequest)

	http.ListenAndServe("localhost:8080", nil)
}

