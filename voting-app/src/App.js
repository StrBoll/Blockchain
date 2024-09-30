import React, { useState } from 'react';
import Terminal, { ColorMode, TerminalOutput } from 'react-terminal-ui';
import './App.css'
const TerminalController = (props = {}) => {
  const [terminalLineData, setTerminalLineData] = useState([
    <TerminalOutput key={1}>Welcome to Blockchain Voting</TerminalOutput>
  ]);

  const handleInput = (terminalInput) => {
    setTerminalLineData([
      ...terminalLineData,
      <TerminalOutput key={terminalLineData.length}>{terminalInput}</TerminalOutput>
    ]);
  };

  return (
    <div className="container">
      <Terminal
        name="React Terminal UI"
        colorMode={ColorMode.Light}
        prompt=">"
        onInput={handleInput}
        height="25em"
        startingInputValue=""
        redBtnCallback={() => alert('Trying to leave? No :)')}
        yellowBtnCallback={() => alert('Sorry, I won\'t minimize')}
        greenBtnCallback={() => alert('Am I not big enough?')}
      >
        {terminalLineData}
      </Terminal>
    </div>
  );
};

export default TerminalController;
