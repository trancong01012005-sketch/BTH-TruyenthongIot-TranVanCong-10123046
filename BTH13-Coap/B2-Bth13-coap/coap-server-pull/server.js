const coap = require('coap');
const server = coap.createServer();

let temperature = 30.0;
let observers = [];

server.on('request', (req, res) => {
  const client = `${req.rsinfo.address}:${req.rsinfo.port}`;

  /* ===== REGISTER OBSERVER ===== */
  if (req.url === '/sensor/temp/observe') {
    console.log('✅ Register observer:', client);

    const exists = observers.find(o =>
      o.ip === req.rsinfo.address && o.port === req.rsinfo.port
    );

    if (!exists) {
      observers.push({
        ip: req.rsinfo.address,
        port: req.rsinfo.port
      });
    }

    res.end(`temp=${temperature}`);
  }

  /* ===== UNREGISTER ===== */
  else if (req.url === '/sensor/temp/unobserve') {
    console.log('❌ Unregister:', client);

    observers = observers.filter(o =>
      o.ip !== req.rsinfo.address || o.port !== req.rsinfo.port
    );

    res.end('Unregistered');
  }

  /* ===== NORMAL GET ===== */
  else if (req.url === '/sensor/temp') {
    res.end(`temp=${temperature}`);
  }

  else {
    res.code = '4.04';
    res.end('Not Found');
  }
});

/* ===== SIMULATE SENSOR ===== */
setInterval(() => {
  temperature += 0.5;
  console.log('\n🌡️ Temperature:', temperature);

  observers.forEach((obs, i) => {
    console.log(`📤 Notify [${i + 1}] → ${obs.ip}:${obs.port}`);

    const req = coap.request({
      host: obs.ip,
      port: obs.port,
      pathname: '/sensor/temp/notify',
      method: 'PUT',
      confirmable: false
    });

    req.write(`temp=${temperature}`);
    req.end();
  });

}, 5000);

server.listen(5683, () => {
  console.log('🚀 CoAP Observer Server running on port 5683');
});
