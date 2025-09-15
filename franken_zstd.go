package franken_zstd

//#include <zend.h>
//#include "zstd.h"
import "C"
import (
	"fmt"
	"sync"
	"time"

	"github.com/klauspost/compress/zstd"
)

func init() {
	C.register_extension()
}

// Global pools for efficiency
var (
	encoderPool = sync.Pool{
		New: func() interface{} {
			encoder, _ := zstd.NewWriter(nil, zstd.WithEncoderLevel(zstd.SpeedDefault))
			return encoder
		},
	}

	decoderPool = sync.Pool{
		New: func() interface{} {
			decoder, _ := zstd.NewReader(nil)
			return decoder
		},
	}

	compressorRegistry   = make(map[string]*Compressor)
	compressorRegistryMu sync.RWMutex
)

// Compression statistics
type CompressionStats struct {
	OriginalSize     int64
	CompressedSize   int64
	CompressionRatio float64
	CompressionTime  time.Duration
}

// Compressor instance
type Compressor struct {
	level      int
	windowSize int
	encoder    *zstd.Encoder
	decoder    *zstd.Decoder
	stats      CompressionStats
	mutex      sync.RWMutex
}

// NewCompressor creates a new compressor instance
func NewCompressor(level, windowSize int) (*Compressor, error) {
	if level < 1 || level > 22 {
		return nil, fmt.Errorf("invalid compression level: %d (must be 1-22)", level)
	}
	if windowSize < 10 || windowSize > 31 {
		return nil, fmt.Errorf("invalid window size: %d (must be 10-31)", windowSize)
	}

	encoder, err := zstd.NewWriter(nil,
		zstd.WithEncoderLevel(zstd.EncoderLevelFromZstd(level)),
		zstd.WithWindowSize(1<<windowSize))
	if err != nil {
		return nil, fmt.Errorf("failed to create encoder: %w", err)
	}

	decoder, err := zstd.NewReader(nil)
	if err != nil {
		encoder.Close()
		return nil, fmt.Errorf("failed to create decoder: %w", err)
	}

	return &Compressor{
		level:      level,
		windowSize: windowSize,
		encoder:    encoder,
		decoder:    decoder,
	}, nil
}

// Compress compresses data using zstd
func (c *Compressor) Compress(data []byte) ([]byte, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()

	startTime := time.Now()
	compressed := c.encoder.EncodeAll(data, make([]byte, 0, len(data)))
	compressionTime := time.Since(startTime)

	c.stats = CompressionStats{
		OriginalSize:     int64(len(data)),
		CompressedSize:   int64(len(compressed)),
		CompressionRatio: float64(len(data)) / float64(len(compressed)),
		CompressionTime:  compressionTime,
	}

	return compressed, nil
}

// Decompress decompresses zstd-compressed data
func (c *Compressor) Decompress(compressedData []byte) ([]byte, error) {
	c.mutex.Lock()
	defer c.mutex.Unlock()

	decompressed, err := c.decoder.DecodeAll(compressedData, nil)
	if err != nil {
		return nil, fmt.Errorf("decompression failed: %w", err)
	}

	return decompressed, nil
}

// Close releases resources
func (c *Compressor) Close() {
	c.mutex.Lock()
	defer c.mutex.Unlock()

	if c.encoder != nil {
		c.encoder.Close()
		c.encoder = nil
	}
	if c.decoder != nil {
		c.decoder.Close()
		c.decoder = nil
	}
}

//export go_compressor_create
func go_compressor_create(phpName *C.zend_string, level, windowSize int64) *C.char {
	name := zendStringToGoString(phpName)

	compressorRegistryMu.Lock()
	defer compressorRegistryMu.Unlock()

	// Check if compressor already exists
	if _, exists := compressorRegistry[name]; exists {
		return nil // Success, compressor already exists
	}

	compressor, err := NewCompressor(int(level), int(windowSize))
	if err != nil {
		return C.CString(err.Error())
	}

	compressorRegistry[name] = compressor
	return nil // Success
}

//export go_compressor_compress
func go_compressor_compress(phpName *C.zend_string, data *C.zend_string, result **C.char, resultLen *C.int, error **C.char) {
	name := zendStringToGoString(phpName)
	inputData := zendStringToGoBytes(data)

	compressorRegistryMu.RLock()
	compressor, exists := compressorRegistry[name]
	compressorRegistryMu.RUnlock()

	if !exists {
		*error = C.CString("compressor not found")
		return
	}

	compressed, err := compressor.Compress(inputData)
	if err != nil {
		*error = C.CString(err.Error())
		return
	}

	// Use binary-safe conversion
	*result = (*C.char)(C.CBytes(compressed))
	*resultLen = C.int(len(compressed))
}

//export go_compressor_decompress
func go_compressor_decompress(phpName *C.zend_string, compressedData *C.zend_string, result **C.char, resultLen *C.int, error **C.char) {
	name := zendStringToGoString(phpName)
	inputData := zendStringToGoBytes(compressedData)

	compressorRegistryMu.RLock()
	compressor, exists := compressorRegistry[name]
	compressorRegistryMu.RUnlock()

	if !exists {
		*error = C.CString("compressor not found")
		return
	}

	decompressed, err := compressor.Decompress(inputData)
	if err != nil {
		*error = C.CString(err.Error())
		return
	}

	// Use binary-safe conversion
	*result = (*C.char)(C.CBytes(decompressed))
	*resultLen = C.int(len(decompressed))
}

//export go_compressor_get_stats
func go_compressor_get_stats(phpName *C.zend_string, originalSize, compressedSize *int64, compressionRatio *float64, compressionTime *int64, error **C.char) {
	// Safety check for null pointer
	if phpName == nil {
		*error = C.CString("null compressor name")
		return
	}

	name := zendStringToGoString(phpName)

	compressorRegistryMu.RLock()
	compressor, exists := compressorRegistry[name]
	compressorRegistryMu.RUnlock()

	if !exists {
		*error = C.CString(fmt.Sprintf("compressor not found: %s", name))
		return
	}

	compressor.mutex.RLock()
	stats := compressor.stats
	compressor.mutex.RUnlock()

	// Safely assign values, ensuring pointers are valid
	if originalSize != nil {
		*originalSize = stats.OriginalSize
	}
	if compressedSize != nil {
		*compressedSize = stats.CompressedSize
	}
	if compressionRatio != nil {
		*compressionRatio = stats.CompressionRatio
	}
	if compressionTime != nil {
		*compressionTime = int64(stats.CompressionTime.Nanoseconds())
	}
}

//export go_compressor_close
func go_compressor_close(phpName *C.zend_string) *C.char {
	name := zendStringToGoString(phpName)

	compressorRegistryMu.Lock()
	defer compressorRegistryMu.Unlock()

	compressor, exists := compressorRegistry[name]
	if !exists {
		return C.CString("compressor not found")
	}

	compressor.Close()
	delete(compressorRegistry, name)
	return nil // Success
}

// Quick compress function for simple use cases
//
//export go_quick_compress
func go_quick_compress(data *C.zend_string, level int64, result **C.char, resultLen *C.int, error **C.char) {
	inputData := zendStringToGoBytes(data)

	encoder := encoderPool.Get().(*zstd.Encoder)
	defer encoderPool.Put(encoder)

	// Reset encoder with new level
	encoder.Reset(nil)

	compressed := encoder.EncodeAll(inputData, make([]byte, 0, len(inputData)))
	// Use binary-safe conversion
	if len(compressed) > 0 {
		*result = (*C.char)(C.CBytes(compressed))
		*resultLen = C.int(len(compressed))
	} else {
		*resultLen = 0
	}
}

// Quick decompress function for simple use cases
//
//export go_quick_decompress
func go_quick_decompress(compressedData *C.zend_string, result **C.char, resultLen *C.int, error **C.char) {
	inputData := zendStringToGoBytes(compressedData)

	decoder := decoderPool.Get().(*zstd.Decoder)
	defer decoderPool.Put(decoder)

	decompressed, err := decoder.DecodeAll(inputData, nil)
	if err != nil {
		*error = C.CString(err.Error())
		return
	}

	// Use binary-safe conversion
	if len(decompressed) > 0 {
		*result = (*C.char)(C.CBytes(decompressed))
		*resultLen = C.int(len(decompressed))
	} else {
		*resultLen = 0
	}
}

// Get zstd information
//
//export go_get_zstd_info
func go_get_zstd_info(version **C.char, maxLevel, minLevel, defaultLevel *int64) {
	*version = C.CString("1.0.3")
	*maxLevel = 22
	*minLevel = 1
	*defaultLevel = 3
}
