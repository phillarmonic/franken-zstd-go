# FrankenPHP Zstd Extension - Build Configuration

# Default recipe
default:
    @just --list

# Variables
binary_output := "frankenphp"
php_lib_path := `php-config --prefix` + "/lib"
cgo_cflags := `php-config --includes`
cgo_ldflags := `php-config --ldflags` + " " + `php-config --libs` + " -L" + php_lib_path

# Generate PHP stubs
generate-stubs:
    @echo "Generating PHP stubs..."
    php `php-config --prefix`/lib/php/build/gen_stub.php zstd.stub.php

# Build the FrankenPHP binary with the zstd extension
build: generate-stubs
    @echo "Building FrankenPHP with zstd extension..."
    CGO_ENABLED=1 \
    XCADDY_GO_BUILD_FLAGS="-ldflags='-w -s'" \
    CGO_CFLAGS="{{cgo_cflags}}" \
    CGO_LDFLAGS="{{cgo_ldflags}}" \
    xcaddy build \
        --output {{binary_output}} \
        --with github.com/dunglas/frankenphp/caddy@v1.9.1 \
        --with github.com/phillarmonic/franken-zstd-go=.

# Test the extension
test: build
    @echo "Testing zstd extension..."
    @echo "Checking extension registration..."
    ./{{binary_output}} php-cli testdata/extension_check.php
    @echo ""
    @echo "Running basic functionality tests..."
    ./{{binary_output}} php-cli testdata/index.php
    @echo ""
    @echo "Running benchmark tests..."
    ./{{binary_output}} php-cli testdata/benchmark.php
    @echo ""
    @echo "✅ All tests completed successfully!"

# Check if extension is properly registered
check-extension: build
    @echo "Checking extension registration..."
    ./{{binary_output}} php-cli testdata/extension_check.php

# Start web server for testing
web-server: build
    @echo "Starting web server on http://127.0.0.1:10000"
    @echo "Test URLs:"
    @echo "  - http://127.0.0.1:10000/web_test.php (Web-specific test)"
    @echo "  - http://127.0.0.1:10000/index.php (Full test suite)"
    @echo "Press Ctrl+C to stop"
    cd testdata && ../{{binary_output}} run --config Caddyfile

# Clean build artifacts
clean:
    @echo "Cleaning build artifacts..."
    rm -f {{binary_output}}
    rm -f zstd_arginfo.h

# Check dependencies
check-deps:
    @command -v php-config >/dev/null 2>&1 || { echo "❌ php-config not found"; exit 1; }
    @command -v xcaddy >/dev/null 2>&1 || { echo "❌ xcaddy not found"; exit 1; }
    @command -v go >/dev/null 2>&1 || { echo "❌ go not found"; exit 1; }
    @echo "✅ All dependencies found"

# Development server
dev: build
    @echo "Starting development server..."
    ./{{binary_output}} run --config ./testdata/Caddyfile

# Benchmark the extension
benchmark: build
    @echo "Running benchmarks..."
    ./{{binary_output}} php ./testdata/benchmark.php

# Download dependencies
deps:
    @echo "Downloading Go dependencies..."
    go mod download
    go mod tidy

# Update version across all files
update-version VERSION:
    @echo "Updating version to {{VERSION}} across all files..."
    @# Update zstd.c - PHP extension version info (line 359)
    sed -i.bak 's/php_info_print_table_row(2, "Extension version", "[^"]*");/php_info_print_table_row(2, "Extension version", "{{VERSION}}");/' zstd.c
    @# Update zstd.c - module entry version (line 373)  
    sed -i.bak 's/"[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*",/"{{VERSION}}",/' zstd.c
    @# Update franken_zstd.go - Go version function (line 309)
    sed -i.bak 's/\*version = C\.CString("[^"]*")/\*version = C.CString("{{VERSION}}")/' franken_zstd.go
    @# Update README.md - documentation examples
    sed -i.bak 's/franken-zstd-go@v[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*/franken-zstd-go@v{{VERSION}}/g' README.md
    sed -i.bak 's/franken-zstd-go v[0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*/franken-zstd-go v{{VERSION}}/g' README.md
    @# Clean up backup files
    rm -f zstd.c.bak franken_zstd.go.bak README.md.bak
    @echo "✅ Version updated to {{VERSION}} in all files"
    @echo ""
    @echo "Files updated:"
    @echo "  - zstd.c (PHP extension version info)"
    @echo "  - franken_zstd.go (Go version function)"
    @echo "  - README.md (documentation examples)"
    @echo ""
    @echo "⚠️  Remember to commit these changes and create a git tag:"
    @echo "   git add -A"
    @echo "   git commit -m 'Bump version to {{VERSION}}'"
    @echo "   git tag v{{VERSION}}"
    @echo "   git push origin main --tags"

# Show current version information
show-version:
    @echo "Current version information:"
    @echo ""
    @echo "📄 zstd.c (PHP extension):"
    @grep -n "Extension version" zstd.c || echo "  Not found"
    @grep -n '"[0-9]\+\.[0-9]\+\.[0-9]\+",' zstd.c || echo "  Not found"
    @echo ""
    @echo "📄 franken_zstd.go (Go function):"
    @grep -n 'C\.CString("[0-9]\+\.[0-9]\+\.[0-9]\+")' franken_zstd.go || echo "  Not found"
    @echo ""
    @echo "📄 README.md (documentation):"
    @grep -n "franken-zstd-go@v[0-9]\+\.[0-9]\+\.[0-9]\+" README.md || echo "  Not found"
    @grep -n "franken-zstd-go v[0-9]\+\.[0-9]\+\.[0-9]\+" README.md || echo "  Not found"

# Publish version with semantic versioning tags
publish-version VERSION:
    #!/usr/bin/env bash
    set -euo pipefail
    
    # Validate version format (x.y.z)
    if ! echo "{{VERSION}}" | grep -qE '^[0-9]+\.[0-9]+\.[0-9]+$'; then
        echo "❌ Invalid version format. Expected: x.y.z (e.g., 1.2.3)"
        exit 1
    fi
    
    # Extract version components
    FULL_VERSION="v{{VERSION}}"
    MAJOR=$(echo "{{VERSION}}" | cut -d. -f1)
    MINOR=$(echo "{{VERSION}}" | cut -d. -f1-2)
    MAJOR_TAG="v${MAJOR}"
    MINOR_TAG="v${MINOR}"
    
    echo "🚀 Publishing version {{VERSION}} with semantic tags..."
    echo ""
    echo "Tags to be created/updated:"
    echo "  📌 Full version:  ${FULL_VERSION}"
    echo "  📌 Minor version: ${MINOR_TAG}"
    echo "  📌 Major version: ${MAJOR_TAG}"
    echo ""
    
    # Check if we're in a git repository
    if ! git rev-parse --git-dir > /dev/null 2>&1; then
        echo "❌ Not in a git repository"
        exit 1
    fi
    
    # Check for uncommitted changes
    if ! git diff-index --quiet HEAD --; then
        echo "⚠️  You have uncommitted changes. Please commit them first."
        echo ""
        echo "Suggested commands:"
        echo "  git add -A"
        echo "  git commit -m 'Bump version to {{VERSION}}'"
        echo ""
        read -p "Continue anyway? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            echo "❌ Aborted"
            exit 1
        fi
    fi
    
    # Create/update full version tag
    echo "📌 Creating full version tag: ${FULL_VERSION}"
    git tag -f "${FULL_VERSION}" -m "Release version {{VERSION}}"
    
    # Create/update minor version tag (force update if exists)
    echo "📌 Creating/updating minor version tag: ${MINOR_TAG}"
    if git tag -l | grep -q "^${MINOR_TAG}$"; then
        echo "   ↳ Updating existing minor tag"
        git tag -f "${MINOR_TAG}" -m "Latest {{VERSION}} in ${MINOR}.x series"
    else
        echo "   ↳ Creating new minor tag"
        git tag "${MINOR_TAG}" -m "Latest {{VERSION}} in ${MINOR}.x series"
    fi
    
    # Create/update major version tag (force update if exists)
    echo "📌 Creating/updating major version tag: ${MAJOR_TAG}"
    if git tag -l | grep -q "^${MAJOR_TAG}$"; then
        echo "   ↳ Updating existing major tag"
        git tag -f "${MAJOR_TAG}" -m "Latest {{VERSION}} in ${MAJOR}.x.x series"
    else
        echo "   ↳ Creating new major tag"
        git tag "${MAJOR_TAG}" -m "Latest {{VERSION}} in ${MAJOR}.x.x series"
    fi
    
    echo ""
    echo "✅ Tags created successfully!"
    echo ""
    echo "📋 Summary:"
    echo "  • ${FULL_VERSION}  - Exact version {{VERSION}}"
    echo "  • ${MINOR_TAG}     - Latest in {{VERSION}} series (moves with patches)"
    echo "  • ${MAJOR_TAG}      - Latest in ${MAJOR}.x.x series (moves with minor/patch)"
    echo ""
    echo "🚀 To push tags to remote:"
    echo "   git push origin --tags"
    echo ""
    echo "🔍 To view all tags:"
    echo "   git tag -l | sort -V"

# Show current git tags
show-tags:
    #!/usr/bin/env bash
    echo "📋 Current git tags (sorted by version):"
    echo ""
    if git tag -l | grep -q .; then
        git tag -l | sort -V | while read tag; do
            commit_hash=$(git rev-list -n 1 $tag 2>/dev/null || echo "invalid")
            commit_date=$(git log -1 --format="%ci" $tag 2>/dev/null || echo "unknown")
            printf "  %-12s %s (%s)\n" "$tag" "$commit_hash" "${commit_date%% *}"
        done
    else
        echo "  No tags found"
    fi
    echo ""
    echo "💡 Use 'just publish-version x.y.z' to create semantic version tags"

# Complete release workflow
release VERSION:
    @echo "🎯 Complete release workflow for version {{VERSION}}"
    @echo ""
    just update-version {{VERSION}}
    @echo ""
    @echo "📝 Files updated. Please review the changes:"
    just show-version
    @echo ""
    @read -p "Commit these changes? (Y/n): " -n 1 -r; \
    echo; \
    if [[ ! $$REPLY =~ ^[Nn]$$ ]]; then \
        git add -A && \
        git commit -m "Bump version to {{VERSION}}" && \
        echo "✅ Changes committed"; \
    else \
        echo "⏭️  Skipping commit"; \
    fi
    @echo ""
    just publish-version {{VERSION}}
