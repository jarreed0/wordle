package yourgamemobile

import (
    "github.com/hajimehoshi/ebiten/v2"
    "github.com/hajimehoshi/ebiten/v2/mobile"

    "../go/"
)

func init() {
    // yourgame.Game must implement ebiten.Game interface.
    // For more details, see
    // * https://pkg.go.dev/github.com/hajimehoshi/ebiten/v2#Game
    mobile.SetGame(&main.g{})
}

// Dummy is a dummy exported function.
//
// gomobile doesn't compile a package that doesn't include any exported function.
// Dummy forces gomobile to compile this package.
func Dummy() {}
