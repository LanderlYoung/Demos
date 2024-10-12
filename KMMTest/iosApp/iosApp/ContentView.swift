import SwiftUI
import Kt

struct ContentView: View {
    @ObservedObject private(set) var viewModel: ViewModel

	var body: some View {
        Text(viewModel.text)
	}
}

extension ContentView {
    class ViewModel: ObservableObject {
        @Published var text = "Loading..."
            init() {
                KMMBridge.mmBenchmark()
                KMMBridge.getLastSuccessfulLaunch { greeting, error in
                    if let greeting = greeting {
                        self.text = greeting
                    } else {
                        self.text = error?.localizedDescription ?? "error"
                    }
                }
            }
    }
}

//struct ContentView_Previews: PreviewProvider {
//	static var previews: some View {
//		ContentView(viewModel: ContentView.ViewModel())
//	}
//}
