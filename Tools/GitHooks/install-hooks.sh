#!/bin/sh
# ProjectTP git 훅 설치. 저장소 어디서든 Git Bash로 실행: sh Tools/GitHooks/install-hooks.sh
# Git LFS 훅(pre-push 등)은 건드리지 않고 pre-commit만 복사한다.
set -e

cd "$(git rev-parse --show-toplevel)"
hooks=$(git rev-parse --git-path hooks)
mkdir -p "$hooks"
cp Tools/GitHooks/pre-commit "$hooks/pre-commit"
chmod +x "$hooks/pre-commit"
echo "pre-commit 훅 설치 완료: $hooks/pre-commit"
