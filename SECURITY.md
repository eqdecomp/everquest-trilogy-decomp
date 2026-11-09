# Security

## Scope
- The test harness and tools compile code with Visual C++ 6 and run it under the Unicorn emulator, and some
  tools run git and other programs. Ways for a fixture, source file or repository content to execute code
  outside that sandbox, or to write outside the workspace, are in scope.
- Running a pull request's tests runs its code on your machine, with no sandbox: read
  it first or use a throw-away VM.
- Not in scope: the original EverQuest client itself.

## Reporting
Please use GitHub's **private vulnerability reporting** on this repository ("Security" tab, "Report a
vulnerability"). Do not open a public issue for a vulnerability, and do not attach game files or memory
dumps. We will answer as soon as we can.

Rights holders can use the same private channel for any concern about this repository.
