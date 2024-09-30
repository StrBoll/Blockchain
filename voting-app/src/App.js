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
        colorMode={ColorMode.Dark}
        prompt=">"
        onInput={handleInput}
        height="25em"
        startingInputValue=""
        redBtnCallback={() => alert('Red button clicked!')}
        yellowBtnCallback={() => alert('Yellow button clicked!')}
        greenBtnCallback={() => alert('Green button clicked!')}
      >
        {terminalLineData}
      </Terminal>
    </div>
  );
};

export default TerminalController;
