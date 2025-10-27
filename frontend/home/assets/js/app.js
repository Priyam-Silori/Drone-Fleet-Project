// app.js

document.addEventListener("DOMContentLoaded", () => {
  console.log("App loaded ✅");

  // If page has drone container (index/home page)
  if (document.querySelector("#drone-container")) {
    fetchDrones();
  }

  // If page has drone form (admin dashboard)
  if (document.querySelector("#droneForm")) {
    setupDroneForm();
  }

  // Setup navigation (if nav links exist)
  setupNavigation();
});

// ===== Fetch Drone Data from Backend =====
async function fetchDrones() {
  try {
    const res = await fetch("http://localhost:5000/api/drones"); // your backend endpoint
    const drones = await res.json();
    displayDrones(drones);
  } catch (err) {
    console.error("Error fetching drones:", err);
  }
}

// ===== Display Drones (for index/dashboard page) =====
function displayDrones(drones) {
  const container = document.querySelector("#drone-container");
  if (!container) return;

  container.innerHTML = "";
  drones.forEach((drone) => {
    const card = document.createElement("div");
    card.classList.add("drone-card");
    card.innerHTML = `
      <h3>${drone.name || drone.id}</h3>
      <p>Battery: ${drone.battery || "N/A"}%</p>
      <p>Status: ${drone.status || "Available"}</p>
      <p>Range: ${drone.range || "—"} km</p>
      <p>Payload: ${drone.payload || "—"} kg</p>
    `;
    container.appendChild(card);
  });
}

// ===== Handle Admin Drone Form =====
function setupDroneForm() {
  const form = document.getElementById("droneForm");
  const tableBody = document.getElementById("droneTable");

  // Load stored drones (fallback if backend not connected)
  let drones = JSON.parse(localStorage.getItem("drones")) || [];

  const renderDrones = () => {
    tableBody.innerHTML = "";
    drones.forEach((drone) => {
      const row = document.createElement("tr");
      row.innerHTML = `
        <td>${drone.id}</td>
        <td>${drone.range}</td>
        <td>${drone.payload}</td>
        <td>${drone.status}</td>
      `;
      tableBody.appendChild(row);
    });
  };

  renderDrones();

  // On form submit
  form.addEventListener("submit", async (e) => {
    e.preventDefault();

    const newDrone = {
      id: document.getElementById("droneId").value.trim(),
      range: parseFloat(document.getElementById("range").value),
      payload: parseFloat(document.getElementById("payload").value),
      status: document.getElementById("status").value,
    };

    try {
      // Try sending to backend first
      const res = await fetch("http://localhost:5000/api/drones/add", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(newDrone),
      });

      if (res.ok) {
        console.log("Drone saved to backend ✅");
      } else {
        console.warn("Backend not responding, saving locally instead.");
        drones.push(newDrone);
        localStorage.setItem("drones", JSON.stringify(drones));
        renderDrones();
      }
    } catch (err) {
      console.error("Error saving drone:", err);
      drones.push(newDrone);
      localStorage.setItem("drones", JSON.stringify(drones));
      renderDrones();
    }

    form.reset();
  });
}

// ===== Navigation Handler =====
function setupNavigation() {
  const navLinks = document.querySelectorAll("nav a");
  navLinks.forEach((link) => {
    link.addEventListener("click", (e) => {
      e.preventDefault();
      const target = e.target.getAttribute("href");
      window.location.href = target;
    });
  });
}
