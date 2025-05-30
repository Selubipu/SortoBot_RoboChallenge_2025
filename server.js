const WebSocket = require('ws');
const fs = require('fs');
const path = require('path');

const FOLDER = "received";
if (!fs.existsSync(FOLDER)) fs.mkdirSync(FOLDER);

const wss = new WebSocket.Server({
  port: 5000,
  perMessageDeflate: false  // 🔧 esențial pentru ESP32-CAM
});

wss.on('connection', function connection(ws, req) {
  const ip = req.socket.remoteAddress;
  console.log(`📥 Client conectat de la ${ip}`);

  ws.on('message', function incoming(data) {
    if (typeof data === 'string') {
      console.log(`💬 Text primit: ${data}`);
      return;
    }

    if (data.length < 2000) {
      console.warn(`⚠️ Imagine ignorată: ${data.length} bytes (prea mică)`);
      return;
    }

    const timestamp = new Date().toISOString().replace(/[:.]/g, "-");
    const filename = path.join(FOLDER, `image_${timestamp}.jpg`);
    fs.writeFileSync(filename, data);
    console.log(`✅ Imagine salvată: ${filename}`);
  });

  ws.on('close', () => {
    console.log(`🔌 Client deconectat`);
  });
});

console.log("🚀 Server WebSocket Node.js activ pe ws://0.0.0.0:5000");
