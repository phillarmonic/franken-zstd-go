package franken_zstd

//#include "zstd.h"
import "C"
import (
	"context"
	"unsafe"

	"github.com/caddyserver/caddy/v2"
	"github.com/caddyserver/caddy/v2/caddyconfig/caddyfile"
	"github.com/dunglas/frankenphp"
	"go.uber.org/zap"
)

func init() {
	caddy.RegisterModule(App{})
	frankenphp.RegisterExtension(unsafe.Pointer(&C.zstd_module_entry))
}

// App is the Caddy app for the zstd extension
type App struct {
	logger *zap.Logger
}

// CaddyModule returns the Caddy module information
func (App) CaddyModule() caddy.ModuleInfo {
	return caddy.ModuleInfo{
		ID:  "zstd",
		New: func() caddy.Module { return new(App) },
	}
}

// Provision sets up the app
func (a *App) Provision(ctx caddy.Context) error {
	a.logger = ctx.Logger()
	return nil
}

// Start starts the app
func (a *App) Start() error {
	a.logger.Info("zstd extension started")
	return nil
}

// Stop stops the app
func (a *App) Stop() error {
	a.logger.Info("zstd extension stopped")
	return nil
}

// UnmarshalCaddyfile implements caddyfile.Unmarshaler
func (a *App) UnmarshalCaddyfile(d *caddyfile.Dispenser) error {
	return nil
}

// getContext returns the current context
func getContext() context.Context {
	return context.Background()
}

// getLogger returns the current logger
func getLogger() *zap.Logger {
	return zap.L()
}

// Interface guards
var (
	_ caddy.Module          = (*App)(nil)
	_ caddy.App             = (*App)(nil)
	_ caddy.Provisioner     = (*App)(nil)
	_ caddyfile.Unmarshaler = (*App)(nil)
)
