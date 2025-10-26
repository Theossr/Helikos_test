# Helikos_test
Pet project appearing as a test task for lab project

---

## Launching with Docker Compose

The project consists of 3 modules:
- **api** — HTTP gateway written in Golang (REST API)  
- **SatelliteSimulation** — intermediate gRPC server counting satellite position data
- **EarthSimulation** — counting magnetic field (and soon maybe other Earth parameters)  

---

### 1. Building images
Before the first launching build all the images:
```bash
docker compose build --no-cache
```

---

### 2. Launching containers
To start all services together:
```bash
docker compose up
```

---

### 3. Checking API availability
Make sure that the API gateway is running:
```bash
curl http://localhost:8080/health
```
Expected response:
```
200/OK
```

---

### 4. Running a simulation
Send a POST request with TLE data to start the simulation:
```bash
curl -X POST http://localhost:8080/simulate   -H "Content-Type: application/json"   -d '{
  "tle": {
    "title": "CUBESAT-3U DEMO",
    "line1": "1 99999U 24001A   24280.50000000  .00001234  00000-0  12345-3 0  9991",
    "line2": "2 99999 097.6000 123.4567 0005000  10.0000 350.0000 15.20000000123456"
  },
  "start_epoch_ms": 1696022400000,
  "duration_s": 5400.0,
  "step_s": 10.0,
  "h_fail_km": 100.0
}'
```
If everything works correctly, the API will return a JSON response containing calculated satellite position and Earth parameters.

---

### 5. Stopping and cleaning up
To stop all containers and remove images, volumes, and networks:
```bash
docker compose down --rmi all --volumes --remove-orphans
```

---
