//
//  ContentView.swift
//  KmmApple
//
//  Created by landerlyoung on 2025/2/22.
//

import SwiftUI
import KMM

struct ContentView: View {
    var body: some View {
        VStack {
            Image(systemName: "globe")
                .imageScale(.large)
                .foregroundStyle(.tint)
            Text(greet())
        }
        .padding()
    }
}

#Preview {
    ContentView()
}

fileprivate func greet() -> String {
    let fusion = Fusion().hello()
    let network = Network().projectName()
    let fibo = Fibonacci().projectName()
    return "Hello, " + fusion
}
