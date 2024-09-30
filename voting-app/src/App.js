import React, { useState, useEffect, useRef } from 'react';
import './terminal.css';

const Terminal = () => {
  const [input, setInput] = useState('');
  const [history, setHistory] = useState([]);
  const [typingText, setTypingText] = useState('');
  const [currentStep, setCurrentStep] = useState(0);
  const [firstName, setFirstName] = useState('');
  const [lastName, setLastName] = useState('');
  const [candidate, setCandidate] = useState('');
  const typingIntervalRef = useRef(null);

  useEffect(() => {
    if (currentStep === 0) {
      typeText(' _Please provide your first name only:');
    } else if (currentStep === 1) {
      typeText(' Now enter your last name:');
    } else if (currentStep === 2) {
      typeText(
        ' Please select from a list of candidates for our 2024 election:\n 1: Donald Trump\n 2: Kamala Harris\n 3: Jill Stein\n 4: Phillip Boll'
      );
    }
    return () => {
      if (typingIntervalRef.current) {
        clearInterval(typingIntervalRef.current);
      }
    };
  }, [currentStep]);

  const typeText = (text) => {
    setTypingText('');
    let i = 0;

    if (typingIntervalRef.current) {
      clearInterval(typingIntervalRef.current);
    }

    typingIntervalRef.current = setInterval(() => {
      if (i < text.length) {
        setTypingText((prev) => prev + text.charAt(i));
        i++;
      } else {
        clearInterval(typingIntervalRef.current);
      }
    }, 50);
  };

  const handleInputSubmit = (e) => {
    if (e.key === 'Enter') {
      if (currentStep === 0) {
        setFirstName(input);
      } else if (currentStep === 1) {
        setLastName(input);
      } else if (currentStep === 2) {
        setCandidate(input);
      }

      setHistory([...history, `> ${input}`]);
      setInput('');

      if (currentStep < 2) {
        setCurrentStep(currentStep + 1);
      }
    }
  };

  const handleInputChange = (e) => {
    setInput(e.target.value);
  };

  return (
    <div className="terminal-container">
      <div className="terminal-header">
        <div className="terminal-buttons">
          <div className="close-btn"></div>
          <div className="minimize-btn"></div>
          <div className="expand-btn"></div>
        </div>
      </div>

      <div className="terminal">
        <div className="terminal-output">
          {history.map((line, index) => (
            <div key={index}>{line}</div>
          ))}
        </div>

        <div className="terminal-prompt">
          <span className="prompt">
            {typingText.split('\n').map((str, index) => (
              <div key={index}>{str}</div>
            ))}
          </span>
        </div>

        {currentStep <= 2 && (
          <div className="terminal-input">
            <span className="prompt"></span>
            <input
              type="text"
              value={input}
              onChange={handleInputChange}
              onKeyDown={handleInputSubmit}
              autoFocus
            />
          </div>
        )}
      </div>
    </div>
  );
};

export default Terminal;
