#!/bin/bash

# 사용법 체크
if [ $# -ne 2 ]; then
    echo "사용법: $0 <소스_폴더> <목적지_폴더>"
    exit 1
fi

SOURCE_DIR="$1"
DEST_DIR="$2"

# 소스 디렉토리 존재 확인
if [ ! -d "$SOURCE_DIR" ]; then
    echo "에러: 소스 폴더 '$SOURCE_DIR'가 존재하지 않습니다."
    exit 1
fi

# 목적지 디렉토리 생성
mkdir -p "$DEST_DIR"

# .h 파일 찾기 및 복사
find "$SOURCE_DIR" -type f -name "*.h" | while read -r file; do
    # ./src/third_party 및 ./src/build 경로 제외
    if [[ "$file" == *"/src/build/"* ]]; then
        continue
    fi
    
    # 상대 경로 계산
    rel_path="${file#$SOURCE_DIR/}"
    dest_file="$DEST_DIR/$rel_path"
    
    # 목적지 디렉토리 생성
    dest_dir=$(dirname "$dest_file")
    mkdir -p "$dest_dir"
    
    # 파일 복사
    cp "$file" "$dest_file"
    echo "복사 완료: $rel_path"
done

echo "모든 .h 파일 복사가 완료되었습니다."
