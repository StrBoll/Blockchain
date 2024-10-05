import React, { useState, useRef } from 'react';
import './terminal.css';

const Terminal = () => {
  const [input, setInput] = useState('');
  const [history, setHistory] = useState([]);
  const [currentStep, setCurrentStep] = useState(0);
  const [firstName, setFirstName] = useState('');
  const [lastName, setLastName] = useState('');
  const [candidate, setCandidate] = useState('');
  const [isComplete, setIsComplete] = useState(false);
  const inputRef = useRef(null);

  const prompts = [
    'Please provide your first name only:',
    'Now enter your last name:',
    'Please select from a list of candidates for our 2024 election:\n 1: Donald Trump\n 2: Kamala Harris\n 3: Jill Stein\n 4: Phillip Boll',
  ];

  const helpPrompt = "Type 'help' for list of commands. \n Mining your vote...";

  const validateNameInput = (name) => /^[a-zA-Z]+$/.test(name);
  const validateCandidateInput = (selection) => ['1', '2', '3', '4'].includes(selection);

  const appendBlockToBlockchain = async (firstName, lastName, candidate) => {
    try {
        const response = await fetch('http://52.14.200.242:18080/appendBlock', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                firstName,
                lastName,
                candidate,
            }),
        });

        const result = await response.text();  // changed it from JSON to plain text expected response
        setHistory((prevHistory) => [...prevHistory, `> Block added: ${result}`]);
    } catch (error) {
        setHistory((prevHistory) => [...prevHistory, `> Error: ${error.message}`]);
    }
};


const showTopCandidate = async () => {
  try {
    const response = await fetch('http://52.14.200.242:18080/topCandidate', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      }
    });

    const result = await response.text();
    setHistory((prevHistory) => [...prevHistory, `> Accessing AWS database... ${result}`]);
  } catch (error) {
    setHistory((prevHistory) => [...prevHistory, `> Error: ${error.message}`]);
  }
};

const printVotesAwsDatabase = async () => {
    try {
      const response = await fetch('http://52.14.200.242:18080/printVotes', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
      });
  
      const result = await response.text();
      setHistory((prevHistory) => [...prevHistory, '> Votes in Blockchain:', result]);
    } catch (error) {
      setHistory((prevHistory) => [...prevHistory, `> Error: ${error.message}`]);
    }
};


const showBlockchain = async () => {
  try {
    const response = await fetch('http://52.14.200.242:18080/showBlockchain', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      }
    });

    const result = await response.text();
    setHistory((prevHistory) => [...prevHistory, `> Accessing AWS database... \n Printing Blockchain... \n ${result}`]);
  } catch (error) {
    setHistory((prevHistory) => [...prevHistory, `> Error: ${error.message}`]);
  }
};
  
  const handleInputSubmit = (e) => {
    if (e.key === 'Enter') {
      if (currentStep === 0) {
        if (validateNameInput(input)) {
          setFirstName(input);
          setHistory([...history, prompts[currentStep], `> ${input}`]);
          setCurrentStep(currentStep + 1);
        } else {
          setHistory([...history, `> ${input}`, 'Please insert one name with letters only']);
        }
      } else if (currentStep === 1) {
        if (validateNameInput(input)) {
          setLastName(input);
          setHistory([...history, prompts[currentStep], `> ${input}`]);
          setCurrentStep(currentStep + 1);
        } else {
          setHistory([...history, `> ${input}`, 'Please insert one name with letters only']);
        }
      } else if (currentStep === 2) {
        if (validateCandidateInput(input)) {
          const candidateMap = {
            '1': 'Donald Trump',
            '2': 'Kamala Harris',
            '3': 'Jill Stein',
            '4': 'Phillip Boll',
          };
          setCandidate(candidateMap[input]);
          setHistory((prevHistory) => [
            ...prevHistory,
            prompts[currentStep],
            `> ${input}`,
            helpPrompt,
          ]);
          setIsComplete(true);
          appendBlockToBlockchain(firstName, lastName, candidateMap[input]);
        } else {
          setHistory([...history, `> ${input}`, 'Please select 1, 2, 3, or 4']);
        }
      }
      setInput('');
    }
  };

  const handleInputChange = (e) => {
    setInput(e.target.value);
  };

  const handleClickTerminal = () => {
    if (inputRef.current) {
      inputRef.current.focus();
    }
  };

  const handleCommand = () => {
    if (input.trim() === 'help') {
      setHistory((prevHistory) => [
        ...prevHistory,
        `> ${input}`,
        "Available commands: \n 'votes - print AWS database of votes' \n 'winning - show who\'s leading the polls",
      ]);
    } else if (input.trim() === 'votes') {
      printVotesAwsDatabase();
    } else if (input.trim() === 'blockchain') {
      showBlockchain();
    } else if(input.trim() === 'winning'){
      showTopCandidate();
    }else {
      setHistory((prevHistory) => [
        ...prevHistory,
        `> ${input}`,
        `Unknown command: '${input}'`,
      ]);
    }
    setInput('');
  };
  
  const renderPromptWithLineBreaks = (text) => {
    return text.split('\n').map((line, index) => (
      <span key={index}>
        {line}
        <br />
      </span>
    ));
  };

  return (
    <div className="terminal-container" onClick={handleClickTerminal}>
      <div className="terminal-header">
        <div className="terminal-buttons">
          <div className="close-btn"></div>
          <div className="minimize-btn"></div>
          <div className="expand-btn"></div>
        </div>
        <div className="terminal-title">Say "help" for commands list</div>
      </div>

      <div className="terminal">
        <div className="terminal-output">
          {history.map((line, index) => (
            <div key={index}>{line}</div>
          ))}
        </div>

        <div className="terminal-prompt">
          {!isComplete && (
            <span className="prompt">
              {renderPromptWithLineBreaks(prompts[currentStep])}
            </span>
          )}
        </div>

        <div className="terminal-input">
          <input
            ref={inputRef}
            type="text"
            value={input}
            onChange={handleInputChange}
            onKeyDown={(e) => {
              if (!isComplete) {
                handleInputSubmit(e);
              } else if (e.key === 'Enter') {
                handleCommand();
              }
            }}
            autoFocus
          />
        </div>
      </div>
    </div>
  );
};

export default Terminal;
