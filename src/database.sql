CREATE TABLE blocks (
    block_number SERIAL PRIMARY KEY,
    previous_hash VARCHAR(255),
    current_hash VARCHAR(255),
    nonce INT,
    transaction_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);


CREATE TABLE votes (
    block_number INT PRIMARY KEY,
    voter_registration INT,
    voter_name VARCHAR(255),
    candidate_vote VARCHAR(255),
    FOREIGN KEY (block_number) REFERENCES blocks(block_number)
);