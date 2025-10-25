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

// ---------------- ADD TASK ----------------
async function addTask(taskId, distance, payload, severity, location) {
  const res = await fetch("http://localhost:8080/addTask", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({
      id: taskId,
      distance: parseInt(distance),
      payload: parseInt(payload),
      severity: parseInt(severity),
      location: parseInt(location),
    }),
  });
  const data = await res.json();
  console.log(data); // "Task added successfully"
}

// ---------------- GET STATUS ----------------
async function getStatus() {
  const res = await fetch("http://localhost:8080/status");
  const status = await res.json();
  console.log(status);
  return status; // { drones: [...], tasks: [...] }
}

// ---------------- GET TASKS ----------------
async function getTasks() {
  const res = await fetch("http://localhost:8080/tasks");
  const tasks = await res.json();
  console.log(tasks);
  return tasks;
}
