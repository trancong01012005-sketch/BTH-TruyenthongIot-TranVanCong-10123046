const SERVER_URL = 'http://localhost:3000';

let socket;
let isConnected = false;

const connectBtn = document.getElementById('connectBtn');
const disconnectBtn = document.getElementById('disconnectBtn');
const statusEl = document.getElementById('status');
const dataDisplay = document.getElementById('dataDisplay');

// Kết nối
connectBtn.addEventListener('click', () => {
  connect();
});

// Ngắt kết nối
disconnectBtn.addEventListener('click', () => {
  disconnect();
});

function connect() {
  socket = io(SERVER_URL);
  
  statusEl.textContent = 'Đang kết nối...';
  statusEl.className = 'waiting';
  
  socket.on('connect', () => {
    console.log('Đã kết nối');
    isConnected = true;
    
    // Cập nhật giao diện
    statusEl.textContent = 'Đã kết nối';
    statusEl.className = 'connected';
    
    connectBtn.style.display = 'none';
    disconnectBtn.style.display = 'inline-block';
    dataDisplay.style.display = 'block';
  });
  
  socket.on('disconnect', () => {
    console.log('Mất kết nối');
    handleDisconnect();
  });
  
  socket.on('sensorData', (data) => {
    console.log('Dữ liệu:', data);
    if (isConnected) {
      document.getElementById('temperature').textContent = data.temperature.toFixed(1);
      document.getElementById('humidity').textContent = data.humidity.toFixed(1);
    }
  });
}

function disconnect() {
  if (socket) {
    socket.disconnect();
  }
  handleDisconnect();
}

function handleDisconnect() {
  isConnected = false;
  
  // Cập nhật giao diện
  statusEl.textContent = 'Đã ngắt kết nối';
  statusEl.className = 'disconnected';
  
  connectBtn.style.display = 'inline-block';
  disconnectBtn.style.display = 'none';
  dataDisplay.style.display = 'none';
  
  // Reset giá trị
  document.getElementById('temperature').textContent = '00';
  document.getElementById('humidity').textContent = '00';
}