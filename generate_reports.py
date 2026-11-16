#!/usr/bin/env python3
"""Regenerate the three Vehicle Monitoring System assignment reports (requires reportlab)."""
from pathlib import Path
from reportlab.lib import colors
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.units import mm
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle

ROOT=Path(__file__).resolve().parent
DOCS=ROOT/"docs"
DOCS.mkdir(exist_ok=True)
styles=getSampleStyleSheet()
styles.add(ParagraphStyle(name="DriveTitle",parent=styles["Title"],textColor=colors.HexColor("#142B4A"),fontSize=21,leading=26,spaceAfter=14))
styles.add(ParagraphStyle(name="DriveHead",parent=styles["Heading2"],textColor=colors.HexColor("#176B87"),spaceBefore=10,spaceAfter=5))
styles.add(ParagraphStyle(name="DriveBody",parent=styles["BodyText"],fontSize=9,leading=13,spaceAfter=6))
def para(text): return Paragraph(text,styles["DriveBody"])
def build(filename,title,sections):
    story=[Paragraph("VEHICLE MONITORING SYSTEM",styles["DriveTitle"]),Paragraph(title,styles["Heading1"]),Spacer(1,8)]
    for heading,body in sections:
        story.append(Paragraph(heading,styles["DriveHead"]))
        if isinstance(body,list):
            t=Table(body,repeatRows=1,colWidths=[48*mm,120*mm])
            t.setStyle(TableStyle([("BACKGROUND",(0,0),(-1,0),colors.HexColor("#142B4A")),("TEXTCOLOR",(0,0),(-1,0),colors.white),("GRID",(0,0),(-1,-1),.4,colors.HexColor("#CBD5DF")),("VALIGN",(0,0),(-1,-1),"TOP"),("FONTSIZE",(0,0),(-1,-1),8),("ROWBACKGROUNDS",(0,1),(-1,-1),[colors.white,colors.HexColor("#EAF1F6")])]))
            story.append(t)
        else: story.append(para(body))
    doc=SimpleDocTemplate(str(DOCS/filename),pagesize=A4,rightMargin=18*mm,leftMargin=18*mm,topMargin=18*mm,bottomMargin=18*mm,title=title)
    doc.build(story)
if __name__=="__main__":
    build("Design_Document.pdf","Design Document",[
      ("Objective","A Linux C socket application models a Vehicle ECU sender and Dashboard ECU receiver. The same simulated telemetry is transmitted using TCP or UDP."),
      ("Architecture","Vehicle ECU generates speed, engine RPM and steering angle values. IPv4 sockets transport newline-delimited TCP records or UDP datagrams to the Dashboard ECU, which parses and displays them."),
      ("Build and run","From the project root run make. Start ./build/dashboard_ecu --protocol udp --bind 127.0.0.1 --port 5005, then ./build/vehicle_ecu --protocol udp --host 127.0.0.1 --port 5005 --count 20. Replace udp with tcp for TCP testing."),
      ("Scope","Signals are simulated, not read from physical sensors. This is an application-layer IPv4 loopback demonstration, not a physical Ethernet PHY, SOME/IP, CAN or safety-certified ECU.")
    ])
    build("Validation_Report.pdf","Validation Report",[
      ("Executed tests",[["Test","Observed result"],["Build","PASS – both applications compiled with GCC C11 warning flags."],["UDP normal operation","PASS – Dashboard received five of five telemetry records."],["TCP normal operation","PASS – Dashboard received five of five telemetry records."],["UDP receiver unavailable","sendto returned success; UDP does not confirm delivery."],["TCP receiver unavailable","Connection refused; sender returned non-zero."],["Invalid IPv4 address","Rejected with exit code 2."]]),
      ("Evidence limitation","The report records actual application-level tests. Genuine packet captures and Wireshark screenshots were not produced in the report-generation runtime because raw packet-capture permission/tools were unavailable. Do not represent them as completed."),
      ("How to capture","On Ubuntu install tcpdump and Wireshark, run scripts/run_experiments.sh, inspect both PCAP files, and save genuine packet screenshots.")
    ])
    build("TCP_UDP_Comparison.pdf","TCP vs UDP Comparison",[
      ("Comparison",[["Feature","TCP / UDP"],["Connection","TCP is connection-oriented; UDP is connectionless."],["Delivery","TCP provides reliable ordered delivery; UDP is best effort."],["Data model","TCP is a byte stream and needs application framing; UDP preserves datagram boundaries."],["Failure","TCP connection setup can report an unavailable listener; UDP send success does not confirm receipt."]]),
      ("Wireshark","Use tcp.port == 5005 and udp.port == 5005. Inspect the TCP SYN/SYN-ACK/ACK handshake, data and FIN sequence, and compare it with individual UDP datagrams."),
      ("Conclusion","TCP suits reliable ordered communication; UDP suits lightweight datagram delivery when the application can tolerate loss or handle it itself. Selection in real automotive systems depends on timing, fault handling and system requirements.")
    ])
    print("Generated three PDFs in docs/.")
