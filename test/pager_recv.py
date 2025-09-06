import sys
from PyQt6.QtWidgets import QApplication, QWidget, QVBoxLayout, QTextEdit
from PyQt6.QtNetwork import QUdpSocket, QHostAddress
from PyQt6.QtGui import QFont
from PyQt6.QtCore import Qt

class UdpReceiverApp(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("PyQt UDP Receiver")
        self.setGeometry(100, 100, 465, 340)

        self.text_edit = QTextEdit(self)
        self.text_edit.setReadOnly(True)

        font = QFont("Monospace")  # You can specify a preferred monospace font like "Courier New", "Consolas", etc.
        font.setStyleHint(QFont.StyleHint.Monospace) # Or QFont.TypeWriter
        self.text_edit.setFont(font)
        self.text_edit.setAlignment(Qt.AlignmentFlag.AlignHCenter)
        self.text_edit.setAlignment(Qt.AlignmentFlag.AlignVCenter)

        self.text_edit.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        layout = QVBoxLayout()
        layout.addWidget(self.text_edit)
        self.setLayout(layout)

        self.udp_socket = QUdpSocket(self)
        # Bind to any available IPv4 address on port 12345
        if not self.udp_socket.bind(QHostAddress.SpecialAddress.AnyIPv4, 6502):
            self.text_edit.append("Error: Could not bind UDP socket.")
            return

        self.udp_socket.readyRead.connect(self.read_pending_datagrams)
        self.text_edit.append("UDP Receiver started on port 12345...")

    def read_pending_datagrams(self):
        while self.udp_socket.hasPendingDatagrams():
            datagram, sender_address, sender_port = self.udp_socket.readDatagram(self.udp_socket.pendingDatagramSize())
            self.text_edit.clear()
            self.print_page(datagram)

    def print_page(self, data: bytes):
        page_str = '      '

        for x in range(16):
            page_str += f'{x:02x} '
        page_str += '\n'

        i = 0
        for x in range(16):
            page_str += '06' + f'{x:x}0: '
            for y in range(16):
                page_str += f'{data[i]:02x} '
                i += 1
            page_str += '\n'

        self.text_edit.setText(page_str)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    receiver = UdpReceiverApp()
    receiver.show()
    sys.exit(app.exec())