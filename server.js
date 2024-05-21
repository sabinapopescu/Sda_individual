const express = require('express');
const app = express();

const PORT = 5501; // Choose any available port, for example 3000

// Serve static files (your website files)
app.use(express.static('/index.html'));

// Start the server
app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
