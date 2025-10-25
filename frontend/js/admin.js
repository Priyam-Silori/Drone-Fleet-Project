// ---------------- LOGIN ----------------
async function login(username, password) {
  const res = await fetch("http://localhost:8080/login", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ username, password }),
  });
  const data = await res.json();
  return data.role; // "admin", "user", or "invalid"
}

// ---------------- ADD DRONE ----------------
async function addDrone(droneId, range, payload) {
  const res = await fetch("http://localhost:8080/addDrone", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({
      id: droneId,
      range: parseInt(range),
      payload: parseInt(payload),
      battery: 100,
      available: true,
      location: 0,
    }),
  });
  const data = await res.json();
  console.log(data); // "Drone added successfully"
}

// ---------------- ASSIGN TASKS ----------------
async function assignTasks() {
  const res = await fetch("http://localhost:8080/assignTasks", {
    method: "POST",
  });
  const data = await res.text();
  console.log(data); // "Tasks assigned successfully"
}

// ---------------- GET DRONES (for admin table) ----------------
async function getDrones() {
  const res = await fetch("http://localhost:8080/drones");
  const drones = await res.json();
  console.log(drones);
  return drones;
}
