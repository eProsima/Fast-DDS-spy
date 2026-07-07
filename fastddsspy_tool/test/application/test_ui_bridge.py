#!/usr/bin/env python3

"""Integration tests for the Fast DDS Spy JSONL bridge."""

from __future__ import annotations

import argparse
import json
import queue
import subprocess
import sys
import threading
import time
from typing import Any


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Test the Fast DDS Spy JSONL bridge.")
    parser.add_argument("--exe", required=True)
    parser.add_argument("--case", choices=("basic", "stream"), required=True)
    return parser.parse_args()


def reader_thread(
    stream,
    target_queue: queue.Queue[Any],
    parser,
) -> None:
    try:
        for raw_line in stream:
            line = raw_line.rstrip("\n")
            if not line:
                continue
            target_queue.put(parser(line))
    finally:
        stream.close()


def parse_event(line: str) -> dict[str, Any]:
    payload = json.loads(line)
    if not isinstance(payload, dict):
        raise ValueError(f"Expected a JSON object, got: {payload!r}")
    return payload


def wait_for_event(
    process: subprocess.Popen[str],
    event_queue: queue.Queue[dict[str, Any]],
    stderr_lines: queue.Queue[str],
    *,
    timeout: float,
    matcher,
    seen: list[dict[str, Any]],
) -> dict[str, Any]:
    deadline = time.time() + timeout
    while time.time() < deadline:
        try:
            event = event_queue.get(timeout=0.2)
        except queue.Empty:
            if process.poll() is not None:
                break
            continue

        seen.append(event)
        if matcher(event):
            return event

    stderr_dump = []
    while not stderr_lines.empty():
        stderr_dump.append(stderr_lines.get())

    hint = ""
    if process.poll() == 127:
        hint = " Hint: source install/setup.bash before running the test so the backend can find workspace libraries."

    raise AssertionError(
        f"Timed out waiting for the expected bridge event. Exit={process.poll()} Seen={seen!r} "
        f"stderr={stderr_dump!r}.{hint}"
    )


def send_line(process: subprocess.Popen[str], line: str) -> None:
    if process.stdin is None:
        raise AssertionError("Process stdin is not available.")
    process.stdin.write(line + "\n")
    process.stdin.flush()


def stop_process(process: subprocess.Popen[str], timeout: float = 10.0) -> None:
    try:
        process.wait(timeout=timeout)
    except subprocess.TimeoutExpired:
        process.terminate()
        try:
            process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait(timeout=5)


def run_basic_case(executable: str) -> None:
    process = subprocess.Popen(
        [executable, "--ui-bridge", "jsonl"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding="utf-8",
        bufsize=1,
    )

    assert process.stdout is not None
    assert process.stderr is not None

    events: queue.Queue[dict[str, Any]] = queue.Queue()
    stderr_lines: queue.Queue[str] = queue.Queue()
    seen_events: list[dict[str, Any]] = []

    threading.Thread(target=reader_thread, args=(process.stdout, events, parse_event), daemon=True).start()
    threading.Thread(target=reader_thread, args=(process.stderr, stderr_lines, lambda line: line), daemon=True).start()

    send_line(process, "help")

    wait_for_event(
        process,
        events,
        stderr_lines,
        timeout=15.0,
        matcher=lambda event: event.get("type") == "output"
        and "Commands available and the information they show:" in event.get("message", ""),
        seen=seen_events,
    )

    send_line(process, "exit")
    stop_process(process)

    if process.returncode != 0:
        stderr_dump = []
        while not stderr_lines.empty():
            stderr_dump.append(stderr_lines.get())
        raise AssertionError(f"Bridge basic case failed with exit code {process.returncode}. stderr={stderr_dump!r}")


def run_stream_case(executable: str) -> None:
    process = subprocess.Popen(
        [executable, "--ui-bridge", "jsonl"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding="utf-8",
        bufsize=1,
    )

    assert process.stdout is not None
    assert process.stderr is not None

    events: queue.Queue[dict[str, Any]] = queue.Queue()
    stderr_lines: queue.Queue[str] = queue.Queue()
    seen_events: list[dict[str, Any]] = []

    threading.Thread(target=reader_thread, args=(process.stdout, events, parse_event), daemon=True).start()
    threading.Thread(target=reader_thread, args=(process.stderr, stderr_lines, lambda line: line), daemon=True).start()

    send_line(process, "show all")

    wait_for_event(
        process,
        events,
        stderr_lines,
        timeout=20.0,
        matcher=lambda event: event.get("type") == "stream_started",
        seen=seen_events,
    )

    send_line(process, "")

    wait_for_event(
        process,
        events,
        stderr_lines,
        timeout=20.0,
        matcher=lambda event: event.get("type") == "stream_stopped",
        seen=seen_events,
    )

    send_line(process, "exit")
    stop_process(process)

    if process.returncode != 0:
        stderr_dump = []
        while not stderr_lines.empty():
            stderr_dump.append(stderr_lines.get())
        raise AssertionError(f"Bridge stream case failed with exit code {process.returncode}. stderr={stderr_dump!r}")


def main() -> int:
    args = parse_args()

    if args.case == "basic":
        run_basic_case(args.exe)
        return 0

    run_stream_case(args.exe)
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:  # pragma: no cover - integration failure path
        print(exc, file=sys.stderr)
        raise
