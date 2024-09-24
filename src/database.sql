CREATE TABLE blocks (
    block_number SERIAL PRIMARY KEY,
    previous_hash VARCHAR(255),
    current_hash VARCHAR(255),
    nonce INT,
    transaction_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);


CREATE TABLE votes (
    vote_hash VARCHAR(255),
    voter_registration INT,
    voterFirst VARCHAR(255),
    VoterLast VARCHAR(255),
    candidate_vote VARCHAR(255),
     FOREIGN KEY (vote_hash) REFERENCES blocks(current_hash)


)


