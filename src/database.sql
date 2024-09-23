CREATE TABLE blocks (
    block_number SERIAL PRIMARY KEY,
    previous_hash VARCHAR(255),
    current_hash VARCHAR(255),
    nonce INT,
    transaction_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
