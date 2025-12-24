const coap = require('coap');
const server = coap.createServer();

let temperature = 30.0;
let observers = [];

console.log('╔════════════════════════════════════════╗');
console.log('║  CoAP Server - PUSH Mode (Observer)   ║');
console.log('╚════════════════════════════════════════╝\n');

server.on('request', (req, res) => {
    console.log(`📨 ${req.method} ${req.url} from ${req.rsinfo.address}`);
    
    if (req.url === '/sensor/temp') {
        // Nếu client đăng ký Observe
        if (req.headers.Observe === 0) {
            console.log('✅ Client registered for Observer');
            console.log(`   Total observers: ${observers.length + 1}\n`);
            
            observers.push(res);
            res.write(`temp=${temperature.toFixed(1)}`);
            
            // Xóa observer khi ngắt kết nối
            res.on('finish', () => {
                const index = observers.indexOf(res);
                if (index > -1) {
                    observers.splice(index, 1);
                    console.log('❌ Observer unregistered');
                    console.log(`   Total observers: ${observers.length}\n`);
                }
            });
        }
        else {
            // GET thông thường
            res.end(`temp=${temperature.toFixed(1)}`);
            console.log(`  → Sent: temp=${temperature.toFixed(1)}\n`);
        }
    }
});

// Thay đổi nhiệt độ và push đến observers mỗi 5 giây
setInterval(() => {
    temperature += (Math.random() - 0.5) * 2;
    temperature = Math.max(20, Math.min(40, temperature));
    
    console.log('════════════════════════════════════════');
    console.log(`🌡️  Temperature updated: ${temperature.toFixed(1)}°C`);
    console.log(`📢 Notifying ${observers.length} observer(s)...`);
    
    observers.forEach((res, index) => {
        try {
            res.write(`temp=${temperature.toFixed(1)}`);
            console.log(`  ✓ Sent to observer #${index + 1}`);
        } catch (err) {
            console.log(`  ✗ Failed to send to observer #${index + 1}`);
        }
    });
    console.log('════════════════════════════════════════\n');
}, 5000);

server.listen(5683, () => {
    console.log('✓ Server started on port 5683');
    console.log('✓ Waiting for observers...\n');
});