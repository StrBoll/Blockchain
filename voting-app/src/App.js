import React, { useState, useRef } from 'react';
import './terminal.css';

const Terminal = () => {
  const [input, setInput] = useState(''); // User input
  const [history, setHistory] = useState([]); // Terminal history
  const [currentStep, setCurrentStep] = useState(0); // Controls the flow of steps
  const [firstName, setFirstName] = useState(''); // Store first name
  const [lastName, setLastName] = useState(''); // Store last name
  const [candidate, setCandidate] = useState(''); // Store candidate selection
  const [isComplete, setIsComplete] = useState(false); // Track completion of all prompts
  const inputRef = useRef(null); // Create a reference for the input field

  // Prompts for each step
  const prompts = [
    'Please provide your first name only:',
    'Now enter your last name:',
    'Please select from a list of candidates for our 2024 election:\n 1: Donald Trump\n 2: Kamala Harris\n 3: Jill Stein\n 4: Phillip Boll',
  ];

  // Commands prompt after completion
  const helpPrompt = "Type 'help' for list of commands.";

  // Validation function for name input (letters only)
  const validateNameInput = (name) => {
    const regex = /^[a-zA-Z]+$/; // Letters only
    return regex.test(name);
  };

  // Validation function for candidate selection (1, 2, 3, or 4)
  const validateCandidateInput = (selection) => {
    return ['1', '2', '3', '4'].includes(selection);
  };

  const handleInputSubmit = (e) => {
    if (e.key === 'Enter') {
      if (currentStep === 0) {
        if (validateNameInput(input)) {
          setFirstName(input); // Store first name
          setHistory([...history, prompts[currentStep], `> ${input}`]); // Add prompt and user input to history
          setCurrentStep(currentStep + 1); // Move to the next step
        } else {
          setHistory([...history, `> ${input}`, 'Please insert one name with letters only']);
        }
      } else if (currentStep === 1) {
        if (validateNameInput(input)) {
          setLastName(input); // Store last name
          setHistory([...history, prompts[currentStep], `> ${input}`]); // Add prompt and user input to history
          setCurrentStep(currentStep + 1); // Move to the next step
        } else {
          setHistory([...history, `> ${input}`, 'Please insert one name with letters only']);
        }
      } else if (currentStep === 2) {
        if (validateCandidateInput(input)) {
          setCandidate(input); // Store candidate selection
          setHistory((prevHistory) => [
            ...prevHistory,
            prompts[currentStep], // Only append the prompt and input once
            `> ${input}`,
            helpPrompt, // Add the help prompt after completion
          ]);
          setIsComplete(true); // Mark completion
        } else {
          setHistory([...history, `> ${input}`, 'Please select 1, 2, 3, or 4']);
        }
      }

      setInput(''); // Clear input field
    }
  };

  const handleInputChange = (e) => {
    setInput(e.target.value);
  };

  const handleClickTerminal = () => {
    // Focus on the input field when clicking on the terminal container
    if (inputRef.current) {
      inputRef.current.focus();
    }
  };

  const handleCommand = () => {
    if (input.trim() === 'help') {
      setHistory((prevHistory) => [
        ...prevHistory,
        `> ${input}`,
        "Available commands: 'help' - Shows the list of commands",
      ]);
    } else {
      setHistory((prevHistory) => [
        ...prevHistory,
        `> ${input}`,
        `Unknown command: '${input}'`,
      ]);
    }

    setInput(''); // Clear input field
  };

  // Helper function to display newlines as <br> in HTML
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
            ref={inputRef} // Attach the ref to the input field
            type="text"
            value={input}
            onChange={handleInputChange}
            onKeyDown={(e) => {
              if (!isComplete) {
                handleInputSubmit(e); // Handle step inputs
              } else if (e.key === 'Enter') {
                handleCommand(); // Handle commands after steps are complete
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
