'use client';

import { useEffect, useRef, useState, useCallback } from 'react';
import { examples } from './examples';

const HELP_SECTIONS = [
  {
    title: 'variables',
    code: `let x = 42;\nlet name = "syrup";\nlet flag = true;`,
  },
  {
    title: 'print',
    code: `print x;       // no newline\nprintln x;     // with newline`,
  },
  {
    title: 'operators',
    code: `+ - * /\n++ --\n+= -= *= /=\n== != < > <= >=\n&& ||`,
  },
  {
    title: 'if / else',
    code: `if (x > 0) {\n  println "pos";\n} else {\n  println "neg";\n}`,
  },
  {
    title: 'while',
    code: `while (x != 0) {\n  println x;\n  x--;\n}`,
  },
  {
    title: 'for',
    code: `for (let i = 0; i < 10; i++) {\n  println i;\n}`,
  },
  {
    title: 'functions',
    code: `def add(a, b) {\n  return a + b;\n}\nprintln add(1, 2);`,
  },
  {
    title: 'comments',
    code: `// this is a comment`,
  },
];

declare global {
  interface Window {
    Module: {
      print?: (text: string) => void;
      printErr?: (text: string) => void;
      _printNoNewline?: (text: string) => void;
      onRuntimeInitialized?: () => void;
      ccall?: (
        name: string,
        returnType: string | null,
        argTypes: string[],
        args: unknown[]
      ) => unknown;
    };
  }
}

export default function Playground() {
  const [code, setCode] = useState(examples[0].code);
  const [output, setOutput] = useState('');
  const [hasError, setHasError] = useState(false);
  const [status, setStatus] = useState<'loading' | 'ready'>('loading');
  const [showExamples, setShowExamples] = useState(false);
  const [showHelp, setShowHelp] = useState(false);
  // Accumulate output as a plain string so print (no newline) and println work correctly
  const outputRef = useRef('');
  const hasErrorRef = useRef(false);
  const dropdownRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    window.Module = {
      // println: Emscripten strips the trailing \n before calling this, so we add it back
      print: (text: string) => {
        outputRef.current += text + '\n';
      },
      // print (no newline): called directly from EM_JS web_print, append as-is
      _printNoNewline: (text: string) => {
        outputRef.current += text;
      },
      printErr: (text: string) => {
        hasErrorRef.current = true;
        outputRef.current += text + '\n';
      },
      onRuntimeInitialized: () => {
        setStatus('ready');
      },
    };

    const script = document.createElement('script');
    script.src = '/syrup.js';
    document.body.appendChild(script);

    return () => {
      document.body.removeChild(script);
    };
  }, []);

  // Close dropdown when clicking outside
  useEffect(() => {
    if (!showExamples) return;
    const handler = (e: MouseEvent) => {
      if (dropdownRef.current && !dropdownRef.current.contains(e.target as Node)) {
        setShowExamples(false);
      }
    };
    document.addEventListener('mousedown', handler);
    return () => document.removeEventListener('mousedown', handler);
  }, [showExamples]);

  const runCode = useCallback(() => {
    outputRef.current = '';
    hasErrorRef.current = false;

    try {
      window.Module.ccall?.('run_source', null, ['string'], [code]);
    } catch (e: unknown) {
      if (e instanceof Error && e.name !== 'ExitStatus') {
        hasErrorRef.current = true;
        outputRef.current += '[wasm error] ' + e.message + '\n';
      }
    }

    setOutput(outputRef.current);
    setHasError(hasErrorRef.current);
  }, [code]);

  const handleKeyDown = useCallback(
    (e: React.KeyboardEvent<HTMLTextAreaElement>) => {
      if (e.key === 'Enter' && (e.ctrlKey || e.metaKey)) {
        e.preventDefault();
        if (status === 'ready') runCode();
      }
    },
    [status, runCode]
  );

  return (
    <div className="flex flex-col h-screen bg-[#1e1e2e] text-[#cdd6f4] font-mono p-4 gap-3">
      <h1 className="text-[1.2rem] text-[#cba6f7] tracking-widest">
        syrup playground
      </h1>
      <span className="text-xs text-[#6c7086]">
        {status === 'loading' ? 'loading wasm...' : 'ready'}
      </span>

      <div className="flex gap-3 flex-1 min-h-0">
        <div className="flex flex-col flex-1 gap-1.5">
          <label className="text-xs text-[#6c7086] uppercase tracking-widest">
            code
          </label>
          <textarea
            className="flex-1 bg-[#181825] border border-[#313244] rounded-md text-[#cdd6f4] font-mono text-sm p-3 resize-none outline-none focus:border-[#cba6f7]"
            value={code}
            onChange={(e) => setCode(e.target.value)}
            onKeyDown={handleKeyDown}
            spellCheck={false}
          />
        </div>

        <div className="flex flex-col flex-1 gap-1.5">
          <label className="text-xs text-[#6c7086] uppercase tracking-widest">
            output
          </label>
          <pre
            className={`flex-1 bg-[#181825] border border-[#313244] rounded-md font-mono text-sm p-3 overflow-y-auto whitespace-pre-wrap break-words ${
              hasError ? 'text-[#f38ba8]' : 'text-[#cdd6f4]'
            }`}
          >
            {output}
          </pre>
        </div>
      </div>

      {showHelp && (
        <div
          className="fixed inset-0 bg-black/60 z-20 flex items-center justify-center"
          onClick={() => setShowHelp(false)}
        >
          <div
            className="bg-[#181825] border border-[#313244] rounded-lg p-6 max-w-lg w-full mx-4 max-h-[80vh] overflow-y-auto"
            onClick={(e) => e.stopPropagation()}
          >
            <div className="flex justify-between items-center mb-4">
              <h2 className="text-[#cba6f7] text-sm uppercase tracking-widest">syntax reference</h2>
              <button
                onClick={() => setShowHelp(false)}
                className="text-[#6c7086] hover:text-[#cdd6f4] text-lg leading-none"
              >
                ✕
              </button>
            </div>
            <div className="flex flex-col gap-4">
              {HELP_SECTIONS.map((s) => (
                <div key={s.title}>
                  <div className="text-xs text-[#6c7086] uppercase tracking-widest mb-1">{s.title}</div>
                  <pre className="bg-[#1e1e2e] border border-[#313244] rounded p-3 text-sm text-[#cdd6f4] whitespace-pre">{s.code}</pre>
                </div>
              ))}
            </div>
          </div>
        </div>
      )}

      <div className="flex items-center gap-2">
        <button
          onClick={runCode}
          disabled={status !== 'ready'}
          className="bg-[#cba6f7] text-[#1e1e2e] font-bold font-mono text-sm px-5 py-1.5 rounded-md cursor-pointer hover:bg-[#b4befe] disabled:bg-[#45475a] disabled:text-[#6c7086] disabled:cursor-default"
        >
          run
        </button>

        <div className="relative" ref={dropdownRef}>
          <button
            onClick={() => setShowExamples((v) => !v)}
            className="border border-[#313244] text-[#cdd6f4] font-mono text-sm px-4 py-1.5 rounded-md cursor-pointer hover:border-[#cba6f7] hover:text-[#cba6f7]"
          >
            examples ▾
          </button>

          {showExamples && (
            <div className="absolute bottom-full mb-1 left-0 bg-[#181825] border border-[#313244] rounded-md overflow-hidden z-10 min-w-48">
              {examples.map((ex) => (
                <button
                  key={ex.name}
                  onClick={() => {
                    setCode(ex.code);
                    setShowExamples(false);
                  }}
                  className="block w-full text-left px-4 py-2 text-sm text-[#cdd6f4] hover:bg-[#313244] hover:text-[#cba6f7]"
                >
                  {ex.name}
                </button>
              ))}
            </div>
          )}
        </div>

        <button
          onClick={() => setShowHelp(true)}
          className="border border-[#313244] text-[#6c7086] font-mono text-sm w-24 h-8 rounded-md cursor-pointer hover:border-[#cba6f7] hover:text-[#cba6f7] flex items-center justify-center"
        >
          syntax
        </button>
      </div>
    </div>
  );
}
