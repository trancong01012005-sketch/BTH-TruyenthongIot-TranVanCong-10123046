const coap = require('coap');
const server = coap.createServer();

/* Giả lập firmware 2KB */
const firmware = Buffer.alloc(2048, 'A');

server.on('request', (req, res) => {
  if (req.url === '/firmware') {
    console.log('Firmware requested');
    res.end(firmware);
  }
});

server.listen(() => {
  console.log('CoAP Blockwise server started on port 5683');
});
